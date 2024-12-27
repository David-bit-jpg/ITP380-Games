//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "SDL2/SDL_image.h"
#include <algorithm>
#include "SpriteComponent.h"
#include "Actor.h"
#include <fstream>
#include "Pellet.h"
#include "PowerPellet.h"
#include "PathNode.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Random.h"

Game::Game()
: mWindow(nullptr)
, mRenderer(nullptr)
, mIsRunning(true)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("ITP Game", 100, 100, 470, 520, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	mAudio = new AudioSystem();

	Random::Init();

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	std::vector<Actor*> copy = mActors;
	for (Actor* actor : copy)
	{
		actor->ProcessInput(state);
	}

	mAudio->ProcessInput(state);

	// Toggles for debug path views
	if (!mPrev1Input && state[SDL_SCANCODE_1])
	{
		mShowGhostPaths = !mShowGhostPaths;
	}

	if (!mPrev2Input && state[SDL_SCANCODE_2])
	{
		mShowGraph = !mShowGraph;
		for (auto p : mPathNodes)
		{
			p->GetComponent<SpriteComponent>()->SetIsVisible(mShowGraph);
		}
	}

	// Test sound spam
	if (state[SDL_SCANCODE_N])
	{
		GetAudio()->PlaySound("EatGhost.wav");
	}

	mPrev1Input = static_cast<bool>(state[SDL_SCANCODE_1]);
	mPrev2Input = static_cast<bool>(state[SDL_SCANCODE_2]);
}

void Game::UpdateGame()
{
	// Compute delta time
	Uint32 tickNow = SDL_GetTicks();
	// Wait until 16ms has elapsed since last frame
	while (tickNow - mTicksCount < 16)
	{
		tickNow = SDL_GetTicks();
	}

	// Get deltaTime in seconds
	float deltaTime = (tickNow - mTicksCount) / 1000.0f;
	// Don't let deltaTime be greater than 0.033f (33 ms)
	if (deltaTime > 0.033f)
	{
		deltaTime = 0.033f;
	}
	mTicksCount = tickNow;

	mAudio->Update(deltaTime);

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete any of the dead actors (which will
	// remove them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	// Detect win state
	if (mPlayer->GetState() != ActorState::Paused && mPellets.empty())
	{
		DoGameWin();
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	DebugDrawPaths();

	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		if (sprite->IsVisible())
		{
			sprite->Draw(mRenderer);
		}
	}

	SDL_RenderPresent(mRenderer);
}

void Game::DebugDrawPaths()
{
	if (mShowGraph)
	{
		SDL_SetRenderDrawColor(mRenderer, 127, 127, 127, 255);
		for (auto p : mPathNodes)
		{
			if (p->GetType() != PathNode::Tunnel)
			{
				for (auto n : p->mAdjacent)
				{
					SDL_RenderDrawLine(mRenderer, static_cast<int>(p->GetPosition().x),
									   static_cast<int>(p->GetPosition().y),
									   static_cast<int>(n->GetPosition().x),
									   static_cast<int>(n->GetPosition().y));
				}
			}
		}
	}

	if (mShowGhostPaths)
	{
		// Now draw ghost paths
		for (auto g : mGhosts)
		{
			g->DebugDrawPath(mRenderer);
		}
	}
}

void Game::LoadData()
{
	// Background
	Actor* a = new Actor(this);
	a->SetPosition(Vector2(234.0f, 258.0f));
	SpriteComponent* s = new SpriteComponent(a, 0);
	s->SetTexture(GetTexture("Assets/Background.png"));

	LoadLevel("Assets/Level.txt");
	LoadPaths("Assets/Paths.txt");

	DoGameIntro();
}

void Game::LoadLevel(const std::string& fileName)
{
	const float STARTX = 34.0f;
	const float STARTY = 34.0f;
	const float SPACING = 16.0f;
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Failed to load level: %s", fileName.c_str());
	}

	size_t row = 0;
	std::string line;
	while (!file.eof())
	{
		std::getline(file, line);
		for (size_t col = 0; col < line.size(); col++)
		{
			Vector2 pos;
			pos.x = STARTX + SPACING * col;
			pos.y = STARTY + SPACING * row;
			char letter = line[col];
			if (letter == 'p')
			{
				Pellet* p = new Pellet(this);
				p->SetPosition(pos);
			}
			else if (letter == 'P')
			{
				PowerPellet* p = new PowerPellet(this);
				p->SetPosition(pos);
			}
			else if (letter == 'M')
			{
				pos.x -= SPACING / 2.0f;
				mPlayer = new PacMan(this);
				mPlayer->SetPosition(pos);
			}
			else if (letter >= '1' && letter <= '4')
			{
				Ghost::Type ghostType = static_cast<Ghost::Type>(letter - '1');
				if (static_cast<int>(ghostType) < GHOST_COUNT)
				{
					mGhosts[ghostType] = new Ghost(this, ghostType);
					mGhosts[ghostType]->SetPosition(pos);
				}
			}
		}
		row++;
	}
}

static bool IsPathNode(char adj)
{
	return adj == 'X' || adj == 'T' || adj == 'G' || adj == 'M' || adj == 'P' ||
		   (adj >= '1' && adj <= '4') || (adj >= 'A' && adj <= 'D');
}

static bool IsPath(char adj)
{
	return IsPathNode(adj) || adj == '*';
}

void Game::LoadPaths(const std::string& fileName)
{
	const float STARTX = 34.0f;
	const float STARTY = 34.0f;
	const float SPACING = 16.0f;
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Failed to load paths: %s", fileName.c_str());
	}

	std::vector<std::vector<PathNode*>> grid;
	std::vector<std::vector<char>> txtGrid;
	size_t row = 0;
	std::string line;
	while (!file.eof())
	{
		grid.emplace_back(std::vector<PathNode*>());
		txtGrid.emplace_back(std::vector<char>());
		std::getline(file, line);
		for (size_t col = 0; col < line.size(); col++)
		{
			Vector2 pos;
			pos.x = STARTX + SPACING * col;
			pos.y = STARTY + SPACING * row;
			char letter = line[col];
			txtGrid.back().emplace_back(letter);
			if (IsPathNode(letter))
			{
				PathNode::Type t = PathNode::Default;
				if (letter == 'T')
				{
					t = PathNode::Tunnel;
				}
				else if (letter == 'G' || letter == 'P' || letter == '3' || letter == '4')
				{
					t = PathNode::Ghost;
				}

				PathNode* p = new PathNode(this, t);
				if (letter == 'M')
				{
					pos.x -= SPACING / 2.0f;
					mPlayer->SetSpawnNode(p);
				}
				else if (letter == 'T')
				{
					if (mTunnelLeft == nullptr)
					{
						pos.x -= SPACING * 1.5f;
						mTunnelLeft = p;
					}
					else
					{
						pos.x += SPACING * 1.5f;
						mTunnelRight = p;
					}
				}
				else if (letter >= '1' && letter <= '4')
				{
					int index = letter - '1';
					if (index < GHOST_COUNT)
					{
						mGhosts[letter - '1']->SetSpawnNode(p);
					}
				}
				else if (letter >= 'A' && letter <= 'D')
				{
					int index = letter - 'A';
					if (index < GHOST_COUNT)
					{
						mGhosts[letter - 'A']->SetScatterNode(p);
					}
				}
				else if (letter == 'P')
				{
					mGhostPen = p;
				}
				p->SetPosition(pos);
				p->mNumber = static_cast<int>(mPathNodes.size());
				mPathNodes.emplace_back(p);
				grid.back().emplace_back(p);
			}
			else
			{
				grid.back().emplace_back(nullptr);
			}
		}
		row++;
	}

	// Now hook up paths
	size_t numRows = grid.size();
	size_t numCols = grid[0].size();
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			char letter = txtGrid[i][j];
			if (IsPathNode(letter))
			{
				// Is there a path to the right?
				if (j < numCols - 1 && IsPath(txtGrid[i][j + 1]))
				{
					for (size_t newJ = j + 1; newJ < numCols; newJ++)
					{
						if (grid[i][newJ] != nullptr)
						{
							grid[i][j]->mAdjacent.emplace_back(grid[i][newJ]);
							grid[i][newJ]->mAdjacent.emplace_back(grid[i][j]);
							break;
						}
					}
				}
				// Is there a path down?
				if (i < numRows - 1 && IsPath(txtGrid[i + 1][j]))
				{
					for (size_t newI = i + 1; newI < numRows; newI++)
					{
						if (grid[newI][j] != nullptr)
						{
							grid[i][j]->mAdjacent.emplace_back(grid[newI][j]);
							grid[newI][j]->mAdjacent.emplace_back(grid[i][j]);
							break;
						}
					}
				}
			}
		}
	}

	// Hook up the tunnels
	PathNode* t1 = nullptr;
	PathNode* t2 = nullptr;
	for (auto p : mPathNodes)
	{
		if (p->GetType() == PathNode::Tunnel)
		{
			if (t1 == nullptr)
			{
				t1 = p;
			}
			else
			{
				t2 = p;
			}
		}
	}

	if (t1 && t2)
	{
		t1->mAdjacent.emplace_back(t2);
		t2->mAdjacent.emplace_back(t1);
	}

	// Now set sprite components to visible or not
	for (auto p : mPathNodes)
	{
		p->GetComponent<SpriteComponent>()->SetIsVisible(mShowGraph);
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName, tex);
	}
	return tex;
}

void Game::Shutdown()
{
	delete mAudio;
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::DoGameIntro()
{
	mAudio->PlaySound("IntroMusic.wav");

	// Tell PacMan to do the intro
	mPlayer->DoGameIntro();
}

void Game::DoGameWin()
{
	mAudio->PlaySound("WinMusic.wav");

	// Tell PacMan to do the intro (this just pauses everything)
	mPlayer->DoGameIntro();
	mPlayer->SetState(ActorState::Paused);
}

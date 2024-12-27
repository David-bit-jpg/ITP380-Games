//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include <iostream>
#include <string>
#include "Block.h"
class Block;
class Player;
class SideBlock;
Game::Game()
{
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = new Renderer(this);

	bool init = mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!init)
	{
		SDL_Log("The renderer failed to initialize");
		return false;
	}

	mAudio = new AudioSystem();

	LoadData();
	mAudio->PlaySound("Music.ogg", true);
	mPlayerSound = mAudio->PlaySound("ShipLoop.ogg", true);
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
	for (auto actor : copy)
	{
		actor->ProcessInput(state);
	}

	mAudio->ProcessInput(state);
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

	// For 3D the games, force delta time to 16ms even if slower
	float deltaTime = 0.016f;
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

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// Precache all the sounds (do not remove)
	mAudio->CacheAllSounds();
	Player* player = new Player(this);
	mPlayer = player;
	Matrix4 pm = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(pm);
	Matrix4 vm = Matrix4::CreateLookAt(EYE, TARGET, DIRECTION);
	mRenderer->SetViewMatrix(vm);
	int cnt = 0;
	for (int i = 0; i <= START_LONG; i += STEPSIZE)
	{
		if (cnt == 4)
		{
			cnt = 0;
		}
		if (cnt == 0)
		{
			SideBlock* sbOne = new SideBlock(this, 0); //right
			sbOne->SetPosition(SBONE + Vector3(static_cast<float>(i), 0.0f, 0.0f));
			sbOne->SetRotation(Math::Pi);

			SideBlock* sbTwo = new SideBlock(this, 0); //left
			sbTwo->SetPosition(SBTWO + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbThree = new SideBlock(this, 5); //bottom
			sbThree->SetPosition(SBTHREE + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbFour = new SideBlock(this, 6); //up
			sbFour->SetPosition(SBFOUR + Vector3(static_cast<float>(i), 0.0f, 0.0f));
		}
		if (cnt == 1)
		{
			SideBlock* sbOne = new SideBlock(this, 1); //right
			sbOne->SetPosition(SBONE + Vector3(static_cast<float>(i), 0.0f, 0.0f));
			sbOne->SetRotation(Math::Pi);

			SideBlock* sbTwo = new SideBlock(this, 1); //left
			sbTwo->SetPosition(SBTWO + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbThree = new SideBlock(this, 5); //bottom
			sbThree->SetPosition(SBTHREE + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbFour = new SideBlock(this, 7); //up
			sbFour->SetPosition(SBFOUR + Vector3(static_cast<float>(i), 0.0f, 0.0f));
		}
		if (cnt == 2)
		{
			SideBlock* sbOne = new SideBlock(this, 2); //right
			sbOne->SetPosition(SBONE + Vector3(static_cast<float>(i), 0.0f, 0.0f));
			sbOne->SetRotation(Math::Pi);

			SideBlock* sbTwo = new SideBlock(this, 2); //left
			sbTwo->SetPosition(SBTWO + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbThree = new SideBlock(this, 5); //bottom
			sbThree->SetPosition(SBTHREE + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbFour = new SideBlock(this, 6); //up
			sbFour->SetPosition(SBFOUR + Vector3(static_cast<float>(i), 0.0f, 0.0f));
		}
		if (cnt == 3)
		{
			SideBlock* sbOne = new SideBlock(this, 0); //right
			sbOne->SetPosition(SBONE + Vector3(static_cast<float>(i), 0.0f, 0.0f));
			sbOne->SetRotation(Math::Pi);

			SideBlock* sbTwo = new SideBlock(this, 0); //left
			sbTwo->SetPosition(SBTWO + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbThree = new SideBlock(this, 5); //bottom
			sbThree->SetPosition(SBTHREE + Vector3(static_cast<float>(i), 0.0f, 0.0f));

			SideBlock* sbFour = new SideBlock(this, 7); //up
			sbFour->SetPosition(SBFOUR + Vector3(static_cast<float>(i), 0.0f, 0.0f));
		}
		cnt++;
	}
	LoadBlocks("Assets/Blocks/1.txt");
	LoadBlocks("Assets/Blocks/2.txt");
	LoadBlocks("Assets/Blocks/3.txt");
	LoadBlocks("Assets/Blocks/4.txt");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mAudio;
	mRenderer->Shutdown();
	delete mRenderer;
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
void Game::AddBlock(Block* b)
{
	mBlocks.push_back(b);
}
void Game::RemoveBlock(Block* b)
{
	std::vector<Block*>::iterator mIndex = std::find(mBlocks.begin(), mBlocks.end(), b);
	if (mIndex != mBlocks.end())
	{
		mBlocks.erase(mIndex);
	}
}
const std::vector<class Block*>& Game::GetBlocks()
{
	return mBlocks;
}
void Game::LoadBlocks(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	int mIndex = 0;
	auto n = filename.find(".");
	auto m = filename.rfind("/");
	std::string numberPart = filename.substr(m + 1, n);
	mIndex = std::stoi(numberPart);
	int row = 0;
	while (std::getline(file, line))
	{
		for (int col = 0; col < line.length(); col++)
		{
			Vector3 pos = Vector3(mIndex * BLOCK_X, -BLOCK_LIMIT + BLOCKSIZE * col,
								  BLOCK_LIMIT - BLOCKSIZE * row);
			char c = line[col];
			if (c == 'A')
			{
				Block* mBlock = new Block(this, 3);
				mBlock->SetPosition(pos);
				AddBlock(mBlock);
			}
			if (c == 'B')
			{
				Block* mBlock = new Block(this, 4);
				mBlock->SetPosition(pos);
				AddBlock(mBlock);
			}
		}
		row++;
	}
}

void Game::LoadBlocksRandom(std::string filename, int x)
{
	std::ifstream file(filename);
	std::string line;
	int row = 0;
	while (std::getline(file, line))
	{
		for (int col = 0; col < line.length(); col++)
		{
			Vector3 pos = Vector3(x * BLOCK_X, -BLOCK_LIMIT + BLOCKSIZE * col,
								  BLOCK_LIMIT - BLOCKSIZE * row);
			char c = line[col];
			if (c == 'A')
			{
				Block* mBlock = new Block(this, 3);
				mBlock->SetPosition(pos);
				AddBlock(mBlock);
			}
			if (c == 'B')
			{
				Block* mBlock = new Block(this, 4);
				mBlock->SetPosition(pos);
				AddBlock(mBlock);
			}
		}
		row++;
	}
}
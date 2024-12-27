#include "Game.h"
#include "Random.h"
#include <algorithm>
#include <vector>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include "Block.h"
#include "Spawner.h"
#include "Player.h"
#include "PlayerMove.h"
#include "SDL2/SDL_mixer.h"
class Actor;
class Player;
class SpriteComponent;
class CollisionComponent;
class Random;
class Block;
class Goomba;
class PlayerMove;
class Spawner;
Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = false;
	mPreviousTime = 0;
	mTimeIncrease = 0;
}

Game::~Game()
{
	Shutdown();
}

bool Game::Initialize()
{
	Random::Init();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		return false;
	}
	mWindow = SDL_CreateWindow("mWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT), 0);
	if (!mWindow)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		return false;
	}
	mIsRunning = true;
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		return false;
	}
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	LoadData();
	return true;
}
void Game::Shutdown()
{
	Mix_CloseAudio();
	IMG_Quit();
	UnloadData();
	mIsRunning = false;
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
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
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	std::vector<class Actor*> mActorsCopy = GetActors();
	for (Actor* actor : mActorsCopy)
	{
		actor->ProcessInput(state);
	}

	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{
	float mCurrentTime = 0;
	while (true)
	{
		mCurrentTime = static_cast<float>(SDL_GetTicks());
		mTimeIncrease = mCurrentTime - mPreviousTime;
		if (mTimeIncrease >= FRAME_FACTOR)
		{
			break;
		}
	}
	mPreviousTime = mCurrentTime;
	float deltaTime = mTimeIncrease / MILLISECOND_FACTOR;
	if (deltaTime > DELTATIME_LIMIT)
	{
		deltaTime = DELTATIME_LIMIT;
	}

	std::vector<class Actor*> mActorsCopy = GetActors();
	for (Actor* actor : mActorsCopy)
	{
		actor->Update(deltaTime);
	}

	std::vector<class Actor*> mActorsDestroy;

	for (Actor* actor : mActorsCopy)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			mActorsDestroy.push_back(actor);
		}
	}

	for (Actor* actor : mActorsDestroy)
	{
		delete actor;
	}
}
void Game::GenerateOutput()
{
	if (SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, MAX_COLOR) != 0)
	{
		return;
	}

	if (SDL_RenderClear(mRenderer) != 0)
	{
		return;
	}
	for (SpriteComponent* sprite : GetSprites())
	{
		if (sprite && sprite->IsVisible())
		{
			sprite->Draw(mRenderer);
		}
	}
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* a)
{
	mActors.push_back(a);
}
void Game::RemoveActor(Actor* actor)
{
	std::vector<Actor*>::iterator mIndex = std::find(mActors.begin(), mActors.end(), actor);
	if (mIndex != mActors.end())
	{
		mActors.erase(mIndex);
	}
}
void Game::AddGoomba(Goomba* g)
{
	mGoombas.push_back(g);
}
void Game::RemoveGoomba(Goomba* g)
{
	std::vector<Goomba*>::iterator mIndex = std::find(mGoombas.begin(), mGoombas.end(), g);
	if (mIndex != mGoombas.end())
	{
		mGoombas.erase(mIndex);
	}
}
void Game::AddSpawner(Spawner* g)
{
	mSpawners.push_back(g);
}
void Game::RemoveSpawner(Spawner* g)
{
	std::vector<Spawner*>::iterator mIndex = std::find(mSpawners.begin(), mSpawners.end(), g);
	if (mIndex != mSpawners.end())
	{
		mSpawners.erase(mIndex);
	}
}
void Game::LoadData()
{
	SetBackgroundSound(GetSound("Assets/Sounds/Music.ogg"));
	Mix_PlayChannel(mBackgroundChannel, GetBackgroundSound(), -1);
	Vector2 mBackgroundPosition;
	mBackgroundPosition.x = static_cast<float>(BACKGROUND_X);
	mBackgroundPosition.y = static_cast<float>(BACKGROUND_Y);
	Actor* mBackgroundActor = new Actor(this);
	mBackgroundActor->SetPosition(mBackgroundPosition);
	SpriteComponent* mBackground = new SpriteComponent(mBackgroundActor);
	mBackground->SetTexture(GetTexture("Assets/Background.png"));
	AddSprite(mBackground);

	std::ifstream file("Assets/Level1.txt");
	std::string line;
	int row = 0;
	while (std::getline(file, line))
	{
		for (int col = 0; col < line.length(); col++)
		{
			char c = line[col];
			Vector2 pos(INITIAL + col * SQUARE_LENGTH, INITIAL + row * SQUARE_LENGTH);
			if (c == 'A')
			{
				Block* mBlock = new Block(this, "Assets/BlockA.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'B')
			{
				Block* mBlock = new Block(this, "Assets/BlockB.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'C')
			{
				Block* mBlock = new Block(this, "Assets/BlockC.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'D')
			{
				Block* mBlock = new Block(this, "Assets/BlockD.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'E')
			{
				Block* mBlock = new Block(this, "Assets/BlockE.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'F')
			{
				Block* mBlock = new Block(this, "Assets/BlockF.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'G')
			{
				Block* mBlock = new Block(this, "Assets/BlockG.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'H')
			{
				Block* mBlock = new Block(this, "Assets/BlockH.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'I')
			{
				Block* mBlock = new Block(this, "Assets/BlockI.png", pos.x, pos.y, row);
				AddBlock(mBlock);
			}
			if (c == 'P')
			{
				mPlayer = new Player(this, pos.x, pos.y);
				SetPlayer(mPlayer);
			}
			if (c == 'Y')
			{
				Spawner* mSpawner = new Spawner(this, pos.x, pos.y);
				AddSpawner(mSpawner);
			}
		}
		row++;
	}
}
void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	for (std::unordered_map<std::string, SDL_Texture*>::iterator itr = mTextureCache.begin();
		 itr != mTextureCache.end(); itr++)
	{
		SDL_DestroyTexture(itr->second);
	}
	mTextureCache.clear();
	for (auto& sound : mSounds)
	{
		Mix_FreeChunk(sound.second);
	}
	mSounds.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName)
{
	if (mTextureCache.find(fileName) != mTextureCache.end()) //in
	{
		return mTextureCache[fileName];
	}
	else
	{
		const char* c = fileName.c_str();
		SDL_Surface* mSurface = IMG_Load(c);
		if (mSurface == nullptr)
		{
			SDL_Log("Texture file failed to load");
			return nullptr;
		}
		SDL_Texture* mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
		SDL_FreeSurface(mSurface);
		return mTexture;
	}
}
void Game::AddSprite(SpriteComponent* sc)
{
	mSprites.push_back(sc);
	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}
void Game::RemoveSprite(SpriteComponent* sc)
{
	std::vector<SpriteComponent*>::iterator mIndex = std::find(mSprites.begin(), mSprites.end(),
															   sc);
	if (mIndex != mSprites.end())
	{
		mSprites.erase(mIndex);
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

const std::vector<class Actor*>& Game::GetActors()
{
	return mActors;
}
const std::vector<class SpriteComponent*>& Game::GetSprites()
{
	return mSprites;
}
const std::vector<class Block*>& Game::GetBlocks()
{
	return mBlocks;
}
const std::vector<class Goomba*>& Game::GetGoombas()
{
	return mGoombas;
}
const std::vector<class Spawner*>& Game::GetSpawners()
{
	return mSpawners;
}
Mix_Chunk* Game::GetSound(const std::string& filename)
{
	auto soundIt = mSounds.find(filename);
	if (soundIt != mSounds.end())
	{
		return soundIt->second;
	}
	Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
	if (!sound)
	{
		return nullptr;
	}
	mSounds[filename] = sound;
	return sound;
}

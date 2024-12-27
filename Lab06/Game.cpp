#include "Game.h"
#include "Random.h"
#include <algorithm>
#include <vector>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include "Player.h"
#include "Soldier.h"
#include "Bush.h"
#include "PlayerMove.h"
#include "SDL2/SDL_mixer.h"
#include "TiledBGComponent.h"
#include "CSVHelper.h"
#include "Collider.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "EnemyComponent.h"
#include "AudioSystem.h"
class AudioSystem;
class EnemyComponent;
class PathNode;
class PathFinder;
class Bush;
class Soldier;
class Actor;
class Player;
class SpriteComponent;
class CollisionComponent;
class Random;
class PlayerMove;

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
	AudioSystem* audio = new AudioSystem();
	mAudioSystem = audio;
	LoadData();
	DoGameIntro();
	return true;
}
void Game::Shutdown()
{
	delete mAudioSystem;
	mAudioSystem = nullptr;
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
	mAudioSystem->Update(deltaTime);
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
	if (mAudioSystem->GetSoundState(mSoundHandle) == SoundState::Stopped)
	{
		if (!mLooping)
		{
			mAudioSystem->PlaySound("MusicLoop.ogg", true);
			mLooping = true;
		}
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
	if (mPlayer != nullptr)
	{
		mPlayer->GetSpriteComponent()->Draw(mRenderer);
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
void Game::AddBush(Bush* c)
{
	mBushs.push_back(c);
}
void Game::RemoveBush(Bush* c)
{
	std::vector<Bush*>::iterator mIndex = std::find(mBushs.begin(), mBushs.end(), c);
	if (mIndex != mBushs.end())
	{
		mBushs.erase(mIndex);
	}
}
void Game::LoadData()
{
	mAudioSystem->CacheAllSounds();
	Actor* mTileActor = new Actor(this);
	mTileActor->SetPosition(Vector2(0.0f, 0.0f));
	PathFinder* pathfinder = new PathFinder(this);
	SetPathFinder(pathfinder);
	TiledBGComponent* mTileComp = new TiledBGComponent(mTileActor);
	mTileComp->SetTexture(GetTexture("Assets/Map/Tiles.png"));
	mTileComp->LoadTileCSV("Assets/Map/Tiles.csv", TILEWIDTH, TILEHEIGHT);
	LoadCSV("Assets/Map/Objects.csv");
}
void Game::LoadCSV(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Failed to load level: %s", fileName.c_str());
	}
	std::string line;
	std::vector<int> mTemp;
	std::getline(file, line);
	while (!file.eof())
	{
		std::getline(file, line);
		if (!line.empty())
		{
			std::vector<std::string> mLineInfo = CSVHelper::Split(line);
			float mPx =
				static_cast<float>(std::stoi(mLineInfo[1]) + std::stoi(mLineInfo[3]) / 2.0f);
			float mPy =
				static_cast<float>(std::stoi(mLineInfo[2]) + std::stoi(mLineInfo[4]) / 2.0f);
			if (mLineInfo[0] == "Player")
			{
				mPlayer = new Player(this, mPx, mPy);
				SetPlayer(mPlayer);
			}
			if (mLineInfo[0] == "Collider")
			{
				float mWidth = static_cast<float>(std::stoi(mLineInfo[3]));
				float mHeight = static_cast<float>(std::stoi(mLineInfo[4]));
				Collider* mCollider = new Collider(this, mWidth, mHeight);
				mCollider->SetPosition(Vector2(mPx, mPy));
			}
			if (mLineInfo[0] == "Bush")
			{
				float mWidth = static_cast<float>(std::stoi(mLineInfo[3]));
				float mHeight = static_cast<float>(std::stoi(mLineInfo[4]));
				Bush* mBush = new Bush(this, mWidth, mHeight);
				mBush->SetPosition(Vector2(mPx, mPy));
			}
			if (mLineInfo[0] == "Soldier")
			{
				PathNode* mStart = GetPathFinder()->GetPathNode(std::stoi(mLineInfo[5]),
																std::stoi(mLineInfo[6]));
				PathNode* mEnd = GetPathFinder()->GetPathNode(std::stoi(mLineInfo[7]),
															  std::stoi(mLineInfo[8]));
				Soldier* soldier = new Soldier(this, mStart, mEnd);
				soldier->SetPosition(Vector2(mPx, mPy));
			}
		}
	}
}
void Game::DoGameIntro()
{
	mSoundHandle = mAudioSystem->PlaySound("MusicStart.ogg");
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
void Game::AddSoldier(Soldier* c)
{
	mSoldiers.push_back(c);
}
void Game::RemoveSoldier(Soldier* c)
{
	std::vector<Soldier*>::iterator mIndex = std::find(mSoldiers.begin(), mSoldiers.end(), c);
	if (mIndex != mSoldiers.end())
	{
		mSoldiers.erase(mIndex);
	}
}
void Game::AddEnemyComponent(EnemyComponent* c)
{
	mEnemyComponents.push_back(c);
}
void Game::RemoveEnemyComponent(EnemyComponent* c)
{
	std::vector<EnemyComponent*>::iterator mIndex = std::find(mEnemyComponents.begin(),
															  mEnemyComponents.end(), c);
	if (mIndex != mEnemyComponents.end())
	{
		mEnemyComponents.erase(mIndex);
	}
}
void Game::AddCollider(Collider* c)
{
	mColliders.push_back(c);
}
void Game::RemoveCollider(Collider* c)
{
	std::vector<Collider*>::iterator mIndex = std::find(mColliders.begin(), mColliders.end(), c);
	if (mIndex != mColliders.end())
	{
		mColliders.erase(mIndex);
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
const std::vector<class Collider*>& Game::GetColliders()
{
	return mColliders;
}
const std::vector<class Bush*>& Game::GetBushs()
{
	return mBushs;
}
const std::vector<class Soldier*>& Game::GetSoldiers()
{
	return mSoldiers;
}
const std::vector<class EnemyComponent*>& Game::GetEnemyComponents()
{
	return mEnemyComponents;
}

#include "Game.h"
#include <algorithm>
#include <vector>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
class WrappingMove;
class Actor;
class Frog;
class SpriteComponent;
class Vehicle;
class CollisionComponent;
class Random;
Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = false;
	mPreviousTime = 0;
	mTimeIncrease = 0;
	mFrog = nullptr;
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
	LoadData();
	return true;
}
void Game::Shutdown()
{
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

	mFrog->ProcessInput(state);

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
	mFrog->Update(deltaTime);
	std::vector<class WrappingMove*> mMovesCopy = GetMoves();
	for (WrappingMove* moves : mMovesCopy)
	{
		moves->Update(deltaTime);
	}
	GetFrog()->Update(deltaTime);
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

void Game::LoadData()
{
	Vector2 mBackgroundPosition;
	mBackgroundPosition.x = static_cast<float>(WINDOW_WIDTH / 2);
	mBackgroundPosition.y = static_cast<float>(WINDOW_HEIGHT / 2);
	Actor* mBackgroundActor = new Actor(this);
	mBackgroundActor->SetPosition(mBackgroundPosition);
	SpriteComponent* mBackground = new SpriteComponent(mBackgroundActor);
	mBackground->SetTexture(GetTexture("Assets/Background.png"));
	AddSprite(mBackground);

	std::ifstream file("Assets/Level.txt");
	std::string line;
	int row = 0;
	while (std::getline(file, line))
	{
		for (int col = 0; col < line.length(); col++)
		{
			char c = line[col];
			Vector2 pos(SQUARE_LENGTH + col * SQUARE_LENGTH, INITIAL_Y + row * SQUARE_LENGTH);
			if (c == 'A')
			{
				Vehicle* mVehicle = new Vehicle(this, "Assets/CarA.png", pos.x, pos.y, row);
				AddVehicle(mVehicle);
				AddActor(mVehicle);
			}
			if (c == 'B')
			{
				Vehicle* mVehicle = new Vehicle(this, "Assets/CarB.png", pos.x, pos.y, row);
				AddVehicle(mVehicle);
				AddActor(mVehicle);
			}
			if (c == 'C')
			{
				Vehicle* mVehicle = new Vehicle(this, "Assets/CarC.png", pos.x, pos.y, row);
				AddVehicle(mVehicle);
				AddActor(mVehicle);
			}
			if (c == 'D')
			{
				Vehicle* mVehicle = new Vehicle(this, "Assets/CarD.png", pos.x, pos.y, row);
				AddVehicle(mVehicle);
				AddActor(mVehicle);
			}
			if (c == 'T')
			{
				Vehicle* mVehicle = new Vehicle(this, "Assets/Truck.png", pos.x, pos.y, row);
				AddVehicle(mVehicle);
				AddActor(mVehicle);
			}
			if (c == 'X')
			{
				Log* mLog = new Log(this, "Assets/LogX.png", pos.x, pos.y, row);
				AddLog(mLog);
				AddActor(mLog);
			}
			if (c == 'Y')
			{
				Log* mLog = new Log(this, "Assets/LogY.png", pos.x, pos.y, row);
				AddLog(mLog);
				AddActor(mLog);
			}
			if (c == 'Z')
			{
				Log* mLog = new Log(this, "Assets/LogZ.png", pos.x, pos.y, row);
				AddLog(mLog);
				AddActor(mLog);
			}
			if (c == 'F')
			{
				mFrog = new Frog(this, "Assets/Frog.png", pos.x, pos.y, row);
			}
		}
		row++;
	}
	std::vector<class Vehicle*> mVehiclesCopy = GetVehicles();
	for (Vehicle* v : mVehiclesCopy)
	{
		v->SetFrog(GetFrog());
	}
	std::vector<class Log*> mLogsCopy = GetLogs();
	for (Log* l : mLogsCopy)
	{
		l->SetFrog(GetFrog());
	}
	CreateGoal();
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
void Game::AddMove(WrappingMove* wm)
{
	mMoves.push_back(wm);
}
void Game::RemoveMove(WrappingMove* wm)
{
	std::vector<WrappingMove*>::iterator mIndex = std::find(mMoves.begin(), mMoves.end(), wm);
	if (mIndex != mMoves.end())
	{
		mMoves.erase(mIndex);
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

void Game::AddLog(Log* l)
{
	mLogs.push_back(l);
}
void Game::RemoveLog(Log* l)
{
	std::vector<Log*>::iterator mIndex = std::find(mLogs.begin(), mLogs.end(), l);
	if (mIndex != mLogs.end())
	{
		mLogs.erase(mIndex);
	}
}

void Game::AddVehicle(Vehicle* v)
{
	mVehicles.push_back(v);
}
void Game::RemoveVehicle(Vehicle* v)
{
	std::vector<Vehicle*>::iterator mIndex = std::find(mVehicles.begin(), mVehicles.end(), v);
	if (mIndex != mVehicles.end())
	{
		mVehicles.erase(mIndex);
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
const std::vector<class WrappingMove*>& Game::GetMoves()
{
	return mMoves;
}

const std::vector<class Vehicle*>& Game::GetVehicles()
{
	return mVehicles;
}
const std::vector<class Log*>& Game::GetLogs()
{
	return mLogs;
}
void Game::CreateGoal()
{
	Actor* goal = new Actor(this);
	goal->SetPosition(Vector2(GOAL_X, GOAL_Y));
	CollisionComponent* mGoalCC = new CollisionComponent(goal);
	mGoalCC->SetSize(SQUARE_LENGTH, SQUARE_LENGTH);
	SetGoal(mGoalCC);
}

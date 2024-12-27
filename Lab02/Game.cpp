#include "Game.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include <algorithm>
#include <vector>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"
class Actor;
class SpriteComponent;
class Asteroid;
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
							   WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

void Game::LoadData()
{
	Vector2 mStarsPosition;
	mStarsPosition.x = static_cast<float>(WINDOW_WIDTH / 2);
	mStarsPosition.y = static_cast<float>(WINDOW_HEIGHT / 2);
	Actor* mStarsActor = new Actor(this);
	mStarsActor->SetPosition(mStarsPosition);
	SpriteComponent* mStars = new SpriteComponent(mStarsActor);
	mStars->SetTexture(GetTexture("Assets/Stars.png"));
	AddSprite(mStars); //Stars

	Ship* mShip = new Ship(this);
	Vector2 center(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	mShip->SetPosition(center);
	AddSprite(mShip->GetSpriteComponent());
	for (int i = 0; i < 10; ++i)
	{
		Asteroid* mAsteroid = new Asteroid(this);
		AddSprite(mAsteroid->GetSpriteComponent());
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
void Game::AddAsteroid(Asteroid* a)
{
	mAsteroids.push_back(a);
}
void Game::RemoveAsteroid(Asteroid* a)
{
	std::vector<Asteroid*>::iterator mIndex = std::find(mAsteroids.begin(), mAsteroids.end(), a);
	if (mIndex != mAsteroids.end())
	{
		mAsteroids.erase(mIndex);
	}
}
const std::vector<class Asteroid*>& Game::GetAsteroids()
{
	return mAsteroids;
}
const std::vector<class Actor*>& Game::GetActors()
{
	return mActors;
}
const std::vector<class SpriteComponent*>& Game::GetSprites()
{
	return mSprites;
}
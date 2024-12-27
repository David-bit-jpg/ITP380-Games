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
#include <iostream>
#include <string>
#include "CollisionComponent.h"
#include "LevelLoader.h"
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

	// On Mac, tell SDL that CTRL+Click should generate a Right Click event
	SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");
	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);

	if (!init)
	{
		SDL_Log("The renderer failed to initialize");
		return false;
	}

	mAudio = new AudioSystem();

	LoadData();
	mTicksCount = SDL_GetTicks();
	InputReplay* ir = new InputReplay(this);
	mInputReplay = ir;
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
	if (state[SDL_SCANCODE_P])
	{
		mInputReplay->StartPlayback(mCurrentLevel);
	}
	int x = 0;
	int y = 0;
	Uint32 mouseButtons = SDL_GetRelativeMouseState(&x, &y);
	Vector2 relativeMouse(x, y);
	mInputReplay->InputPlayback(state, mouseButtons, relativeMouse);
	std::vector<Actor*> copy = mActors;
	for (auto actor : copy)
	{
		actor->ProcessInput(state, mouseButtons, relativeMouse);
	}

	mAudio->ProcessInput(state);
	bool reloadPressed = state[SDL_SCANCODE_F5];
	if (reloadPressed && !mIfReloadPressed)
	{
		mLevelWantToLoad = mCurrentLevel;
	}
	mIfReloadPressed = reloadPressed;
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
	mInputReplay->Update(deltaTime);
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
	if (!mLevelWantToLoad.empty())
	{
		UnloadData();
		for (auto collider : mColliders)
		{
			delete collider;
		}
		for (auto door : mDoors)
		{
			delete door;
		}
		mInputReplay->StopPlayback();
		mAudio->StopAllSounds();
		mBluePortal = nullptr;
		mOrangePortal = nullptr;
		mCurrentLevel = mLevelWantToLoad;
		LevelLoader::Load(this, mLevelWantToLoad);
		mLevelWantToLoad = "";
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
	Matrix4 pm = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(pm);
	mCurrentLevel = "Assets/Lab11.json";
	LevelLoader::Load(this, mCurrentLevel);
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
	delete mInputReplay;
	SDL_Quit();
}

void Game::ReloadLevel()
{
	mLevelWantToLoad = mCurrentLevel;
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
void Game::AddColliders(Actor* collider)
{
	mColliders.emplace_back(collider);
}
void Game::RemoveColliders(Actor* collider)
{
	auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
	if (iter != mColliders.end())
	{
		mColliders.erase(iter);
	}
}
void Game::AddDoors(class Door* door)
{
	mDoors.emplace_back(door);
}
void Game::RemoveDoors(class Door* door)
{
	auto iter = std::find(mDoors.begin(), mDoors.end(), door);
	if (iter != mDoors.end())
	{
		mDoors.erase(iter);
	}
}
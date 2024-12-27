#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "Player.h"
#include "AudioSystem.h"
#include "Portal.h"
#include "CollisionComponent.h"
#include "InputReplay.h"
#include "PortalGun.h"
#include "Door.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif
class PortalGun;
class Player;
class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void EmRunIteration()
	{
		if (!mIsRunning)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void Shutdown();
	void ReloadLevel();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddColliders(class Actor* collider);
	void RemoveColliders(class Actor* collider);
	void AddDoors(class Door* door);
	void RemoveDoors(class Door* door);

	AudioSystem* GetAudio() { return mAudio; }
	class Renderer* GetRenderer() { return mRenderer; }
	std::vector<class Actor*> GetColliders() const { return mColliders; }
	std::vector<class Door*> GetDoors() const { return mDoors; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

	Player* GetPlayer() const { return mPlayer; }
	void SetPlayer(Player* p) { mPlayer = p; }
	SoundHandle GetSoundHandle() const { return mSound; }
	void SetSoundHandle(SoundHandle sh) { mSound = sh; }
	class Portal* GetBluePortal() const { return mBluePortal; }
	class Portal* GetOrangePortal() const { return mOrangePortal; }
	void SetBluePortal(Portal* p) { mBluePortal = p; }
	void SetOrangePortal(Portal* p) { mOrangePortal = p; }
	void SetGun(PortalGun* p) { mPortalGun = p; }
	PortalGun* GetGun() const { return mPortalGun; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mColliders;
	std::vector<class Door*> mDoors;

	std::string mCurrentLevel;

	InputReplay* mInputReplay = nullptr;
	PortalGun* mPortalGun = nullptr;

	class Portal* mBluePortal = nullptr;
	class Portal* mOrangePortal = nullptr;

	class Renderer* mRenderer = nullptr;

	AudioSystem* mAudio = nullptr;
	SoundHandle mSound;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;
	Player* mPlayer = nullptr;

	float mPauseTimer = 8.5f;
	bool mStarted = false;

	std::string mLevelWantToLoad;
	bool mIfReloadPressed = false;
};

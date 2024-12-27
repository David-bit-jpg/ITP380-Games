#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "Player.h"
#include "AudioSystem.h"
#include "HeightMap.h"
#include "Enemy.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Enemy;
class Player;
class HeightMap;
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

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	AudioSystem* GetAudio() { return mAudio; }
	class Renderer* GetRenderer() { return mRenderer; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

	Player* GetPlayer() const { return mPlayer; }
	Enemy* GetEnemy() const { return mEnemy; }
	HeightMap* GetHeightMap() const { return mHeightMap; }
	SoundHandle GetSoundHandle() const { return mSound; }
	void SetSoundHandle(SoundHandle sh) { mSound = sh; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;
	HeightMap* mHeightMap = nullptr;
	Enemy* mEnemy = nullptr;

	class Renderer* mRenderer = nullptr;

	AudioSystem* mAudio = nullptr;
	SoundHandle mSound;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;
	Player* mPlayer = nullptr;

	float mPauseTimer = 8.5f;
	bool mStarted = false;
};

#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "Player.h"
#include "AudioSystem.h"
#include "Block.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif
#include "SideBlock.h"
class SideBlock;
class Player;
class Block;
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
	void LoadBlocks(std::string filename);
	void LoadBlocksRandom(std::string filename, int x);

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddBlock(Block* b);
	void RemoveBlock(Block* b);

	AudioSystem* GetAudio() { return mAudio; }
	const std::vector<class Block*>& GetBlocks();
	class Renderer* GetRenderer() { return mRenderer; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;
	const float BLOCKSIZE = 25.0f;
	const float BLOCK_LIMIT = 237.5f;
	const float BLOCK_X = 1000.0f;
	const int START_LONG = 3500;
	const int STEPSIZE = 500;
	const Vector3 EYE = Vector3(-300, 0, 0);
	const Vector3 TARGET = Vector3(20, 0, 0);
	const Vector3 DIRECTION = Vector3::UnitZ;
	const Vector3 SBONE = Vector3(0.0f, 500.0f, 0.0f);
	const Vector3 SBTWO = Vector3(0.0f, -500.0f, 0.0f);
	const Vector3 SBTHREE = Vector3(0.0f, 0.0f, -500.0f);
	const Vector3 SBFOUR = Vector3(0.0f, 0.0f, 500.0f);

	Player* GetPlayer() const { return mPlayer; }
	SoundHandle GetPlayerSound() const { return mPlayerSound; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Block*> mBlocks;

	class Renderer* mRenderer = nullptr;

	AudioSystem* mAudio = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;
	SoundHandle mPlayerSound;
	Player* mPlayer = nullptr;
};

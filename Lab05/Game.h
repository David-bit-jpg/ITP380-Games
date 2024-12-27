#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include "Math.h"
#include "AudioSystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Game
{
	static const int GHOST_COUNT = 4;

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

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
	AudioSystem* GetAudio() { return mAudio; }

	void LoadLevel(const std::string& fileName);
	void LoadPaths(const std::string& fileName);

	std::vector<class PathNode*>& GetPathNodes() { return mPathNodes; }
	std::vector<class PowerPellet*>& GetPowerPellets() { return mPowerPellets; }
	std::vector<class Pellet*>& GetPellets() { return mPellets; }

	class PacMan* GetPlayer() const { return mPlayer; }
	class PathNode* GetTunnelLeft() const { return mTunnelLeft; }
	class PathNode* GetTunnelRight() const { return mTunnelRight; }
	class PathNode* GetGhostPen() const { return mGhostPen; }

	std::array<class Ghost*, GHOST_COUNT>& GetGhosts() { return mGhosts; }

private:
	std::vector<class PathNode*> mPathNodes;
	std::vector<class PowerPellet*> mPowerPellets;
	std::vector<class Pellet*> mPellets;
	class PacMan* mPlayer = nullptr;
	class PathNode* mTunnelLeft = nullptr;
	class PathNode* mTunnelRight = nullptr;
	class PathNode* mGhostPen = nullptr;

	std::array<class Ghost*, GHOST_COUNT> mGhosts = {};

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void DebugDrawPaths();
	void LoadData();
	void UnloadData();
	void DoGameIntro();
	void DoGameWin();

	AudioSystem* mAudio = nullptr;

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount = 0;
	bool mIsRunning;

	bool mShowGraph = false;
	bool mShowGhostPaths = true;
	bool mPrev1Input = false;
	bool mPrev2Input = false;
};

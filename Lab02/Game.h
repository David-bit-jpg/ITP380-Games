#pragma once
#include "SDL2/SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
class Actor;
class SpriteComponent;
class Asteroid;
class Game
{
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	float mTimeIncrease;
	float mPreviousTime;
	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class Asteroid*> mAsteroids;
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	const float MILLISECOND_FACTOR = 1000.0f;
	const int FRAME_FACTOR = 16;
	const float DELTATIME_LIMIT = 0.033f;
	const Uint8 MAX_COLOR = 255;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	std::unordered_map<std::string, SDL_Texture*> mTextureCache;

public:
	Game();
	~Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(Actor* a);
	void RemoveActor(Actor* a);
	SDL_Texture* GetTexture(std::string fileName);
	void AddSprite(SpriteComponent* sc);
	void RemoveSprite(SpriteComponent* sc);
	void AddAsteroid(Asteroid* a);
	void RemoveAsteroid(Asteroid* a);
	const std::vector<class Asteroid*>& GetAsteroids();
	const std::vector<class Actor*>& GetActors();
	const std::vector<class SpriteComponent*>& GetSprites();
};

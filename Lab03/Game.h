#pragma once
#include "SDL2/SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include "WrappingMove.h"
#include "Frog.h"
#include "Vehicle.h"
#include "Log.h"
class Actor;
class SpriteComponent;
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
	std::vector<class WrappingMove*> mMoves;
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Log*> mLogs;
	const float WINDOW_WIDTH = 448.0f;
	const float WINDOW_HEIGHT = 512.0f;
	const float MILLISECOND_FACTOR = 1000.0f;
	const int FRAME_FACTOR = 16;
	const float DELTATIME_LIMIT = 0.033f;
	const Vector2 TO_RIGHT = Vector2(1, 0);
	const Vector2 TO_LEFT = Vector2(-1, 0);
	const Uint8 MAX_COLOR = 255;

	const float SQUARE_LENGTH = 32.0f;
	const float INITIAL_Y = 80.0f;
	const float GOAL_Y = 80.0f;
	const float GOAL_X = 224.0f;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	std::unordered_map<std::string, SDL_Texture*> mTextureCache;
	Frog* mFrog;
	void CreateGoal();
	CollisionComponent* mGoal = nullptr;

public:
	Game();
	~Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(Actor* a);
	void RemoveActor(Actor* a);
	void AddVehicle(Vehicle* v);
	void RemoveVehicle(Vehicle* v);
	void AddLog(Log* l);
	void RemoveLog(Log* l);
	SDL_Texture* GetTexture(std::string fileName);
	void AddSprite(SpriteComponent* sc);
	void AddMove(WrappingMove* wm);
	void RemoveMove(WrappingMove* wm);
	void RemoveSprite(SpriteComponent* sc);
	const std::vector<class Actor*>& GetActors();
	const std::vector<class SpriteComponent*>& GetSprites();
	const std::vector<class WrappingMove*>& GetMoves();
	const std::vector<class Vehicle*>& GetVehicles();
	const std::vector<class Log*>& GetLogs();
	Frog* GetFrog() const { return mFrog; }
	CollisionComponent* GetGoal() const { return mGoal; }
	void SetGoal(CollisionComponent* g) { mGoal = g; }
};

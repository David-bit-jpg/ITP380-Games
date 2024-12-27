#pragma once
#include "SDL2/SDL.h"
#include "Actor.h"
#include "Block.h"
#include "Goomba.h"
#include "Spawner.h"
#include "SpriteComponent.h"
#include "PlayerMove.h"
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <map>
#include "SDL2/SDL_mixer.h"
class Goomba;
class Actor;
class SpriteComponent;
class Block;
class Player;
class Goomba;
class Spawner;
class PlayerMove;
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
	std::vector<class Block*> mBlocks;
	std::vector<class Goomba*> mGoombas;
	std::vector<class Spawner*> mSpawners;
	const float WINDOW_WIDTH = 600.0f;
	const float WINDOW_HEIGHT = 448.0f;
	const float MILLISECOND_FACTOR = 1000.0f;
	const float BACKGROUND_X = 3392.0f;
	const float BACKGROUND_Y = 224.0f;
	const int FRAME_FACTOR = 16;
	const float DELTATIME_LIMIT = 0.033f;
	const Vector2 TO_RIGHT = Vector2(1, 0);
	const Vector2 TO_LEFT = Vector2(-1, 0);
	const Uint8 MAX_COLOR = 255;

	const float SQUARE_LENGTH = 32.0f;
	const float INITIAL = 16.0f;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	Vector2 mCameraPos;
	Player* mPlayer = nullptr;
	std::unordered_map<std::string, SDL_Texture*> mTextureCache;
	PlayerMove* mPlayerMovement = nullptr;
	std::map<std::string, Mix_Chunk*> mSounds;
	Mix_Chunk* mBackgroundSound = nullptr;
	int mBackgroundChannel = -1;

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
	void AddBlock(Block* b);
	void RemoveBlock(Block* b);
	void AddGoomba(Goomba* g);
	void RemoveGoomba(Goomba* g);
	void AddSpawner(Spawner* g);
	void RemoveSpawner(Spawner* g);
	const std::vector<class Actor*>& GetActors();
	const std::vector<class SpriteComponent*>& GetSprites();
	const std::vector<class Block*>& GetBlocks();
	const std::vector<class Goomba*>& GetGoombas();
	const std::vector<class Spawner*>& GetSpawners();
	const Vector2& GetCameraPos() { return mCameraPos; }
	void SetCameraPos(Vector2 s) { mCameraPos = s; }
	Player* GetPlayer() const { return mPlayer; }
	void SetPlayer(Player* p) { mPlayer = p; }
	PlayerMove* GetPlayerMovement() const { return mPlayerMovement; }
	void SetPlayerMovement(PlayerMove* p) { mPlayerMovement = p; }
	Mix_Chunk* GetSound(const std::string& filename);
	Mix_Chunk* GetBackgroundSound() const { return mBackgroundSound; }
	void SetBackgroundSound(Mix_Chunk* p) { mBackgroundSound = p; }
	int GetBackgroundChannel() const { return mBackgroundChannel; }
};

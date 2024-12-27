#pragma once
#include "SDL2/SDL.h"
#include "Actor.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "PlayerMove.h"
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <map>
#include "Bush.h"
#include "Soldier.h"
#include "Collider.h"
#include "SDL2/SDL_mixer.h"
#include "PathFinder.h"
#include "EnemyComponent.h"
#include "AudioSystem.h"
class AudioSystem;
class EnemyComponent;
class PathFinder;
class Bush;
class Soldier;
class Actor;
class SpriteComponent;
class Player;
class TiledBGComponent;
class PlayerMove;
class Collider;
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
	std::vector<class Collider*> mColliders;
	std::vector<class Bush*> mBushs;
	std::vector<class Soldier*> mSoldiers;
	std::vector<class EnemyComponent*> mEnemyComponents;

	const float WINDOW_WIDTH = 512.0f;
	const float WINDOW_HEIGHT = 448.0f;
	const float MILLISECOND_FACTOR = 1000.0f;
	const int FRAME_FACTOR = 16;
	const float DELTATIME_LIMIT = 0.033f;
	const Vector2 TO_RIGHT = Vector2(1, 0);
	const Vector2 TO_LEFT = Vector2(-1, 0);
	const Uint8 MAX_COLOR = 255;

	Vector2 mCameraPos;
	Player* mPlayer = nullptr;
	std::unordered_map<std::string, SDL_Texture*> mTextureCache;
	PlayerMove* mPlayerMovement = nullptr;
	std::map<std::string, Mix_Chunk*> mSounds;
	Mix_Chunk* mBackgroundSound = nullptr;
	int mBackgroundChannel = -1;
	const int TILEWIDTH = 32;
	const int TILEHEIGHT = 32;
	PathFinder* mPathFinder = nullptr;
	AudioSystem* mAudioSystem = nullptr;
	SoundHandle mSoundHandle;
	bool mLooping = false;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void LoadCSV(const std::string& fileName);
	void DoGameIntro();

public:
	Game();
	~Game();

	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(Actor* a);
	void RemoveActor(Actor* a);

	AudioSystem* GetAudio() const { return mAudioSystem; };
	SDL_Texture* GetTexture(std::string fileName);
	void AddSprite(SpriteComponent* sc);
	void RemoveSprite(SpriteComponent* sc);
	void AddCollider(Collider* c);
	void RemoveCollider(Collider* c);
	void AddBush(Bush* c);
	void RemoveBush(Bush* c);
	void AddSoldier(Soldier* c);
	void RemoveSoldier(Soldier* c);
	void AddEnemyComponent(EnemyComponent* c);
	void RemoveEnemyComponent(EnemyComponent* c);

	const std::vector<class Actor*>& GetActors();
	const std::vector<class SpriteComponent*>& GetSprites();
	const std::vector<class Collider*>& GetColliders();
	const std::vector<class Bush*>& GetBushs();
	const std::vector<class Soldier*>& GetSoldiers();
	const std::vector<class EnemyComponent*>& GetEnemyComponents();

	const Vector2& GetCameraPos() { return mCameraPos; }
	void SetCameraPos(Vector2 s) { mCameraPos = s; }
	Player* GetPlayer() const { return mPlayer; }
	void SetPlayer(Player* p) { mPlayer = p; }
	PathFinder* GetPathFinder() const { return mPathFinder; }
	void SetPathFinder(PathFinder* p) { mPathFinder = p; }
	PlayerMove* GetPlayerMovement() const { return mPlayerMovement; }
	void SetPlayerMovement(PlayerMove* p) { mPlayerMovement = p; }
	Mix_Chunk* GetBackgroundSound() const { return mBackgroundSound; }
	void SetBackgroundSound(Mix_Chunk* p) { mBackgroundSound = p; }

	int GetBackgroundChannel() const { return mBackgroundChannel; }
};

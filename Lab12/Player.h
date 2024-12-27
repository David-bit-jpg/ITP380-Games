#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "HUD.h"
class HUD;
class HealthComponent;
class CollisionComponent;
class CameraComponent;
class PlayerMove;
class Game;
class Player : public Actor
{
private:
	PlayerMove* mPlayerMove = nullptr;
	CameraComponent* mCameraComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	HealthComponent* mHealthComponent = nullptr;
	bool mHasGun = false;
	HUD* mHUD = nullptr;

	float const CC_X = 50.0f;
	float const CC_Y = 100.0f;
	float const CC_Z = 50.0f;

	Vector3 mInitialPos;

public:
	Player(Game* game);
	~Player();

	CameraComponent* GetCameraComponent() const { return mCameraComponent; };
	PlayerMove* GetPlayerMove() const { return mPlayerMove; }
	CollisionComponent* GetPlayerCollisionComponent() const { return mCollisionComponent; }
	Vector3 GetInitialPos() const { return mInitialPos; }
	void SetInitialPos(Vector3 i) { mInitialPos = i; }
	HealthComponent* GetHealthComponent() const { return mHealthComponent; }

	bool HasGun() const { return mHasGun; }
	void SetGun(bool c) { mHasGun = c; }
	HUD* GetHUD() const { return mHUD; }
};

#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "HUD.h"
class PlayerMove;
class HUD;
class CollisionComponent;
class MeshComponent;
class Game;
class Player : public Actor
{
private:
	MeshComponent* mMeshComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	PlayerMove* mPlayerMove = nullptr;
	HUD* mHUD = nullptr;

	const float CCX = 40.0f;
	const float CCY = 25.0f;
	const float CCZ = 15.0f;

public:
	Player(Game* game);
	~Player();

	PlayerMove* GetPlayerMove() const { return mPlayerMove; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	HUD* GetHUD() const { return mHUD; }
};

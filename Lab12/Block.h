#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class MeshComponent;
class CollisionComponent;
class Game;
class Block : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;

	float const CC_X = 1.0f;
	float const CC_Y = 1.0f;
	float const CC_Z = 1.0f;
	float const SCALE = 64.0f;

public:
	Block(Game* game);
	~Block();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	MeshComponent* GetMeshComponent() const { return mMeshComponent; }
};

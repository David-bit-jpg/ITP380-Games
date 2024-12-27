#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
class CollisionComponent;
class MeshComponent;
class Game;
class Block : public Actor
{
private:
	void OnUpdate(float deltaTime) override;

	MeshComponent* mMeshComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	bool mIsExploding = false;

	const float CCX = 1.0f;
	const float CCY = 1.0f;
	const float CCZ = 1.0f;
	const float THRESHOLD = 2000.0f;

public:
	Block(Game* game, size_t textureIndex);
	~Block();

	bool IsExplode() const { return mIsExploding; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
};

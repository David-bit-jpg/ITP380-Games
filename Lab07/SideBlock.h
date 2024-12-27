#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
class CollisionComponent;
class MeshComponent;
class Game;
class SideBlock : public Actor
{
private:
	void OnUpdate(float deltaTime) override;
	MeshComponent* mMeshComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;

	const float SCALE = 500.0f;
	const float THRESHOLD = 2000.0f;

public:
	SideBlock(Game* game, size_t textureIndex);
	~SideBlock();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
};

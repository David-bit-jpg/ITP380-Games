#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Block.h"
#include <vector>
class MoveComponent;
class CollisionComponent;
class MeshComponent;
class Game;
class Block;
class Bullet : public Actor
{
private:
	void OnUpdate(float deltaTime) override;
	void Explode(Vector3 x, Block* bb, std::vector<Block*>& mBlocks);

	MeshComponent* mMeshComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	MoveComponent* mMoveComponent = nullptr;
	float mTimer = 0.0f;

	const float CCX = 10.0f;
	const float CCY = 10.0f;
	const float CCZ = 10.0f;
	const float FORWARD_SPEED = 900.0f * GetGame()->GetPlayer()->GetPlayerMove()->GetMultipler();
	const float RANGE = 50.0f;

public:
	Bullet(Game* game);
	~Bullet();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
};

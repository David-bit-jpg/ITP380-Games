#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
class CollisionComponent;
class SpriteComponent;
class Block : public Actor
{
private:
	CollisionComponent* mCollisionComponent{};
	SpriteComponent* mSpriteComponent;
	float const BLOCK_SIZE = 32.0f;
	Vector2 mInitialPos;
	Actor* mOwner{};

public:
	Block(Game* game, std::string s, float x, float y, int row);
	~Block();
	void OnUpdate(float deltaTime) override;
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	SpriteComponent* GetSpriteComponent() const { return mSpriteComponent; }
	Actor* GetOwner() const { return mOwner; }
};

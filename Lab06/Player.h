#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
class AnimatedSprite;
class CollisionComponent;
class SpriteComponent;
class Player : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	AnimatedSprite* mSpriteComponent = nullptr;
	
	float const PLAYER_COLLIDER_SIZE = 20.0f;

public:
	Player(Game* game, float x, float y);
	~Player();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	AnimatedSprite* GetSpriteComponent() const { return mSpriteComponent; }
	void SetSpriteComponent(AnimatedSprite* c) { mSpriteComponent = c; }
};

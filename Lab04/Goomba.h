#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
class CollisionComponent;
class SpriteComponent;
class AnimatedSprite;
class GoombaMove;
class Goomba : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	AnimatedSprite* mSpriteComponent = nullptr;
	float const GOOMBA_SIZE = 32.0f;
	Vector2 mInitialPos;
	GoombaMove* mGoombaMove = nullptr;
	bool mIsStompped = false;

public:
	Goomba(Game* game, float x, float y);
	~Goomba();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	AnimatedSprite* GetSpriteComponent() const { return mSpriteComponent; }
	void SetSpriteComponent(AnimatedSprite* c) { mSpriteComponent = c; }
	bool GetStomp() const { return mIsStompped; }
	void SetStomp(bool c) { mIsStompped = c; }
};

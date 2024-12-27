#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include "Effect.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
class Effect;
class EnemyComponent;
class CollisionComponent;
class SpriteComponent;

class Bush : public Actor
{
public:
	Bush(Game* game, float width, float height);
	~Bush();

	EnemyComponent* GetEnemyComponent() const { return mEnemyComponent; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
	EnemyComponent* mEnemyComponent = nullptr;
	SpriteComponent* mSpriteComponent = nullptr;

	const float BUSH_SIZE = 32.0f;
};

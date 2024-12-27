#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "SoldierAI.h"
#include "EnemyComponent.h"
class EnemyComponent;
class SoldierAI;
class CollisionComponent;
class SpriteComponent;
class AnimatedSprite;
class Soldier : public Actor
{
public:
	Soldier(Game* game, PathNode* start, PathNode* end);
	~Soldier();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	EnemyComponent* GetEnemyComponent() const { return mEnemyComponent; }
	AnimatedSprite* GetAnimatedSprite() const { return mSpriteComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
	EnemyComponent* mEnemyComponent = nullptr;
	const float SOLDIER_SIZE = 32.0f;
	AnimatedSprite* mSpriteComponent = nullptr;
	SoldierAI* mAI = nullptr;
};

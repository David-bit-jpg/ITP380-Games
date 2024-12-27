#pragma once
#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "CollisionComponent.h"
class CollisionComponent;
class Sword : public Actor
{
public:
	Sword(Game* game);
	~Sword();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;

	const float SWORD_SIZE = 20.0f;
};

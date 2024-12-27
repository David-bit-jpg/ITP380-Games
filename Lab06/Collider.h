#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
class CollisionComponent;

class Collider : public Actor
{
public:
	Collider(Game* game, float width, float height);
	~Collider();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
};

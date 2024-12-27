#include "Sword.h"
#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "CollisionComponent.h"
class CollisionComponent;
Sword::Sword(Game* game)
: Actor(game)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(SWORD_SIZE, SWORD_SIZE);
	mCollisionComponent = cc;
}

Sword::~Sword()
{
}

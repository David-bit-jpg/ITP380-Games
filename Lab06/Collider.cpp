#include "Collider.h"
#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "CollisionComponent.h"
class CollisionComponent;
Collider::Collider(Game* game, float width, float height)
: Actor(game)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(width, height);
	mCollisionComponent = cc;
	GetGame()->AddCollider(this);
}

Collider::~Collider()
{
	GetGame()->RemoveCollider(this);
}

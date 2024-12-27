#include "Bush.h"
#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Effect.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
class Effect;
class EnemyComponent;
class CollisionComponent;
class SpriteComponent;
Bush::Bush(Game* game, float width, float height)
: Actor(game)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(BUSH_SIZE, BUSH_SIZE);
	mCollisionComponent = cc;
	EnemyComponent* ec = new EnemyComponent(this);
	ec->SetHitPoint(1);
	ec->SetOnDeathCallback([this]() {
		new Effect(GetGame(), GetPosition(), "BushDeath", "BushDie.wav");
		GetGame()->GetPathFinder()->SetIsBlocked(static_cast<size_t>(GetPosition().y / BUSH_SIZE),
												 static_cast<size_t>(GetPosition().x / BUSH_SIZE),
												 false);
	});
	mEnemyComponent = ec;
	SpriteComponent* sprite = new SpriteComponent(this, 150);
	sprite->SetTexture(GetGame()->GetTexture("Assets/Bush.png"));
	mSpriteComponent = sprite;
	GetGame()->AddBush(this);
}

Bush::~Bush()
{
	GetGame()->RemoveBush(this);
}

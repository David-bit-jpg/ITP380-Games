#include "Soldier.h"
#include "Actor.h"
#include <algorithm>
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
Soldier::Soldier(Game* game, PathNode* start, PathNode* end)
: Actor(game)
{
	SoldierAI* ai = new SoldierAI(this);
	ai->Setup(start, end);
	ai->SetSoldier(this);
	mAI = ai;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(SOLDIER_SIZE, SOLDIER_SIZE);
	mCollisionComponent = cc;
	EnemyComponent* ec = new EnemyComponent(this);
	AnimatedSprite* sprite = new AnimatedSprite(this, 200);
	sprite->LoadAnimations("Assets/Soldier");
	sprite->SetAnimation("WalkDown");
	sprite->SetAnimFPS(5.0f);
	mSpriteComponent = sprite;
	ec->SetHitPoint(2);
	ec->SetOnDamageCallback([this]() {
		new Effect(GetGame(), GetPosition(), "Hit", "EnemyHit.wav");
		this->mAI->SetStunned(true);
	});
	ec->SetOnDeathCallback([this]() {
		new Effect(GetGame(), GetPosition(), "Death", "EnemyDie.wav");
	});
	mEnemyComponent = ec;
	GetGame()->AddSoldier(this);
}

Soldier::~Soldier()
{
	GetGame()->RemoveSoldier(this);
}

#include "EnemyComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include <vector>
#include <functional>
class Game;
class Actor;
class CollisionComponent;
EnemyComponent::EnemyComponent(Actor* owner, int drawOrder)
: Component(owner)
{
	GetGame()->AddEnemyComponent(this);
	mCollisionComponent = GetOwner()->GetComponent<CollisionComponent>();
}

EnemyComponent::~EnemyComponent()
{
	GetGame()->RemoveEnemyComponent(this);
}
void EnemyComponent::TakeDamage()
{
	if (mTimePassed >= THRESHOLD) //if the time passed since last time it's called
	{
		mTimePassed = 0.0f; //reset timer
		mHitPoint -= 1;		//-1 health

		if (mHitPoint <= 0)
		{
			if (mOnDeath)
			{
				mOnDeath();
			}
			GetOwner()->SetState(ActorState::Destroy);
		}
		else
		{
			if (mOnDamage)
			{
				mOnDamage();
			}
		}
	}
}
void EnemyComponent::Update(float deltaTime)
{
	mTimePassed += deltaTime;
}
void EnemyComponent::SetOnDamageCallback(std::function<void()> callback)
{
	mOnDamage = callback;
}

void EnemyComponent::SetOnDeathCallback(std::function<void()> callback)
{
	mOnDeath = callback;
}
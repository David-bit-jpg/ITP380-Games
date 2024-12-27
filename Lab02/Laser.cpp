#include "Laser.h"
#include <string>
#include "Actor.h"
#include "Asteroid.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "Math.h"
class MoveComponent;
class SpriteComponent;
class Game;
class Asteroid;

Laser::Laser(Game* game)
: Actor(game)
{
	mSpriteComponent = new SpriteComponent(this);
	GetSpriteComponent()->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));
	GetGame()->AddSprite(mSpriteComponent);
	mMoveComponent = new MoveComponent(this);
	GetMoveComponent()->SetForwardSpeed(MOVESPEED);
}

Laser::~Laser()
{
	GetGame()->RemoveSprite(mSpriteComponent);
}
MoveComponent* Laser::GetMoveComponent() const
{
	return mMoveComponent;
}
SpriteComponent* Laser::GetSpriteComponent() const
{
	return mSpriteComponent;
}
void Laser::OnUpdate(float deltaTime)
{
	Actor::OnUpdate(deltaTime);
	mLifeTime += deltaTime;
	if (mLifeTime >= LIFETIME_FACTOR)
	{
		mLifeTime = 0.0f;
		SetState(ActorState::Destroy);
		return;
	}
	for (Asteroid* asteroid : GetGame()->GetAsteroids())
	{
		float mDistance = Vector2::Distance(GetPosition(), asteroid->GetPosition());
		if (mDistance <= MIN_DISTANCE)
		{
			asteroid->SetState(ActorState::Destroy);
			SetState(ActorState::Destroy);
			return;
		}
	}
}

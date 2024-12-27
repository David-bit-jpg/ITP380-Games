#include <string>
#include "DeadFrog.h"
#include "SDL2/SDL.h"
class MoveComponent;
class Actor;
class Frog;
class SpriteComponent;
class Vehicle;
class CollisionComponent;
class Random;
class Game;

DeadFrog::DeadFrog(Game* game)
: Actor(game)
{
	mSpriteComponent = new SpriteComponent(this);
	GetSpriteComponent()->SetTexture(GetGame()->GetTexture("Assets/Dead.png"));
	GetGame()->AddSprite(mSpriteComponent);
}

DeadFrog::~DeadFrog()
{
	GetGame()->RemoveSprite(mSpriteComponent);
}
SpriteComponent* DeadFrog::GetSpriteComponent() const
{
	return mSpriteComponent;
}
void DeadFrog::OnUpdate(float deltaTime)
{
	Actor::OnUpdate(deltaTime);
	mLifeTime += deltaTime;
	if (mLifeTime >= LIFETIME_FACTOR)
	{
		mLifeTime = 0.0f;
		SetState(ActorState::Destroy);
		return;
	}
}

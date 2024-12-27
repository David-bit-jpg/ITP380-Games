#include "Asteroid.h"
#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "Math.h"
class MoveComponent;
class SpriteComponent;
class Game;

Asteroid::Asteroid(Game* game)
: Actor(game)
{
	mSpriteComponent = new SpriteComponent(this);
	GetSpriteComponent()->SetTexture(GetGame()->GetTexture("Assets/Asteroid.png"));

	mMoveComponent = new MoveComponent(this);
	GetMoveComponent()->SetForwardSpeed(MOVESPEED);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	Vector2 minPos(0.0f, 0.0f);
	Vector2 maxPos(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetPosition(Random::GetVector(minPos, maxPos));
	GetGame()->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}
MoveComponent* Asteroid::GetMoveComponent() const
{
	return mMoveComponent;
}
SpriteComponent* Asteroid::GetSpriteComponent() const
{
	return mSpriteComponent;
}
void Asteroid::OnUpdate(float deltaTime)
{
	Actor::OnUpdate(deltaTime);
	Vector2 pos = GetPosition();
	if (pos.x < 0)
	{
		pos.x = static_cast<float>(WINDOW_WIDTH - 1);
	}
	else if (pos.x >= static_cast<float>(WINDOW_WIDTH))
	{
		pos.x = 0;
	}
	if (pos.y < 0)
	{
		pos.y = static_cast<float>(WINDOW_HEIGHT - 1);
	}
	else if (pos.y >= static_cast<float>(WINDOW_HEIGHT))
	{
		pos.y = 0;
	}
	SetPosition(pos);
}

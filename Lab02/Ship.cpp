#include "Ship.h"
#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Laser.h"
class MoveComponent;
class SpriteComponent;
class Game;

Ship::Ship(Game* game)
: Actor(game)
{
	mIsMoving = false;
	mSpriteComponent = new SpriteComponent(this);
	mMoveComponent = new MoveComponent(this);
}

Ship::~Ship()
{
}
MoveComponent* Ship::GetMoveComponent() const
{
	return mMoveComponent;
}
SpriteComponent* Ship::GetSpriteComponent() const
{
	return mSpriteComponent;
}
void Ship::OnProcessInput(const Uint8* keyState)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	mIsMoving = false;
	if (keyState[SDL_SCANCODE_W])
	{
		forwardSpeed += FORWARD_SPEED;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		forwardSpeed -= FORWARD_SPEED;
	}
	if (forwardSpeed != 0)
	{
		mIsMoving = true;
	}

	if (keyState[SDL_SCANCODE_A])
	{
		angularSpeed += ANGULAR_SPEED;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		angularSpeed -= ANGULAR_SPEED;
	}
	if (keyState[SDL_SCANCODE_SPACE])
	{
		if (mCooldown <= 0.0f)
		{
			Laser* mLaser = new Laser(GetGame());
			mLaser->SetPosition(GetPosition());
			mLaser->SetRotation(GetRotation());
			mCooldown = COOLDOWN_FACTOR;
		}
	}
	GetMoveComponent()->SetForwardSpeed(forwardSpeed);
	GetMoveComponent()->SetAngularSpeed(angularSpeed);
	if (!(this->CheckMovement()))
		GetSpriteComponent()->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));
	else
		GetSpriteComponent()->SetTexture(GetGame()->GetTexture("Assets/ShipThrust.png"));
}

void Ship::OnUpdate(float deltaTime)
{
	Actor::OnUpdate(deltaTime);
	if (mCooldown > 0.0f)
	{
		mCooldown -= deltaTime;
	}
}

#include "Vehicle.h"
#include <string>
#include "Game.h"
#include "Math.h"
class WrappingMove;
class Actor;
class Game;
class Frog;
class SpriteComponent;
class Vehicle;
class CollisionComponent;
class Random;
class Game;

Vehicle::Vehicle(Game* game, std::string s, float x, float y, int row)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mVCollision = new CollisionComponent(this);
	WrappingMove* vehicleMovement = new WrappingMove(this);
	if (!(strcmp("Assets/Truck.png", s.c_str()) == 0))
	{
		mVCollision->SetSize(CAR_COLLIDER_SIZE, CAR_COLLIDER_SIZE);
	}
	if (strcmp("Assets/Truck.png", s.c_str()) == 0)
	{
		mVCollision->SetSize(T_COLLIDER_SIZE_X, T_COLLIDER_SIZE_Y);
	}
	vehicleMovement->SetForwardSpeed(VEHICLE_SPEED);
	if (row % 2 == 0)
		vehicleMovement->SetDirection(TO_RIGHT);
	if (row % 2 != 0)
		vehicleMovement->SetDirection(TO_LEFT);
	GetGame()->AddMove(vehicleMovement);
	mCollisionComponent = mVCollision;
	mWrappingMove = vehicleMovement;
	mFrog = nullptr;
	mNormalSpeed = GetWrappingMove()->GetForwardSpeed();
	SpriteComponent* sprite = new SpriteComponent(this);
	sprite->SetTexture(GetGame()->GetTexture(s));
	GetGame()->AddSprite(sprite);
}

Vehicle::~Vehicle()
{
	GetGame()->RemoveVehicle(this);
}

void Vehicle::OnUpdate(float deltaTime)
{
	mMoveD = GetWrappingMove()->GetDirection();
	mFrogPos = GetFrog()->GetPosition();
	Vector2 toFrog = mFrogPos - GetPosition();
	toFrog.Normalize();
	mMoveD.Normalize();
	float dot = Vector2::Dot(mMoveD, toFrog);
	float angle = Math::Acos(dot);

	if (angle <= Math::Pi / 6)
	{
		GetWrappingMove()->SetForwardSpeed(mNormalSpeed / 2.0f);
	}
	else
	{
		GetWrappingMove()->SetForwardSpeed(mNormalSpeed);
	}
	std::vector<Vehicle*> vehicles = GetGame()->GetVehicles();
	for (Vehicle* other : vehicles)
	{
		if (this != other)
		{
			if (GetPosition().y == other->GetPosition().y)
			{
				if (GetCollisionComponent()->Intersect(other->GetCollisionComponent()))
				{
					Vector2 offset;
					CollSide collSide = GetCollisionComponent()->GetMinOverlap(
						other->GetCollisionComponent(), offset);
					if (collSide == CollSide::Left)
					{
						if (mMoveD.x == 1)
						{
							SetPosition(Vector2(other->GetPosition().x -
													GetCollisionComponent()->GetWidth(),
												GetPosition().y));
						}
						if (mMoveD.x == -1)
						{
							other->SetPosition(
								Vector2(GetPosition().x + GetCollisionComponent()->GetWidth(),
										GetPosition().y));
						}
					}
					if (collSide == CollSide::Right)
					{
						if (mMoveD.x == 1)
						{
							other->SetPosition(
								Vector2(GetPosition().x - GetCollisionComponent()->GetWidth(),
										GetPosition().y));
						}
						if (mMoveD.x == -1)
						{
							SetPosition(Vector2(other->GetPosition().x +
													GetCollisionComponent()->GetWidth(),
												GetPosition().y));
						}
					}
				}
			}
		}
	}
}
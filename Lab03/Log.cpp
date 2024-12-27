#include "Log.h"
#include <string>
#include "Math.h"
#include "Game.h"
class WrappingMove;
class Actor;
class Game;
class Frog;
class SpriteComponent;
class Vehicle;
class CollisionComponent;
class Random;

Log::Log(Game* game, std::string s, float x, float y, int row)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mLCollision = new CollisionComponent(this);
	WrappingMove* logMovement = new WrappingMove(this);
	if (strcmp("Assets/LogX.png", s.c_str()) == 0)
	{
		mLCollision->SetSize(LOG_COLLIDER_X, LOG_COLLIDER_HEIGHT);
	}
	if (strcmp("Assets/LogY.png", s.c_str()) == 0)
	{
		mLCollision->SetSize(LOG_COLLIDER_Y, LOG_COLLIDER_HEIGHT);
	}
	if (strcmp("Assets/LogZ.png", s.c_str()) == 0)
	{
		mLCollision->SetSize(LOG_COLLIDER_Z, LOG_COLLIDER_HEIGHT);
	}
	logMovement->SetForwardSpeed(LOG_SPEED);
	if (row % 2 == 0)
		logMovement->SetDirection(TO_RIGHT);
	if (row % 2 != 0)
		logMovement->SetDirection(TO_LEFT);
	GetGame()->AddMove(logMovement);
	mCollisionComponent = mLCollision;
	mWrappingMove = logMovement;
	mFrog = nullptr;
	SetCollisionComponent(mLCollision);
	SetWrappingMove(logMovement);
	mNormalSpeed = GetWrappingMove()->GetForwardSpeed();
	SpriteComponent* sprite = new SpriteComponent(this);
	sprite->SetTexture(GetGame()->GetTexture(s));
	GetGame()->AddSprite(sprite);
}

Log::~Log()
{
	GetGame()->RemoveLog(this);
}
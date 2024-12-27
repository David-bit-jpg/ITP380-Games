#include "WrappingMove.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
class MoveComponent;
class Actor;
class Game;
WrappingMove::WrappingMove(Actor* actor)
: MoveComponent(actor)
{
	GetGame()->AddMove(this);
	mDeltatime = 0.0f;
}

WrappingMove::~WrappingMove()
{
	GetGame()->RemoveMove(this);
}
void WrappingMove::SetDirection(const Vector2& direction)
{
	mDirection = direction;
}

Vector2 WrappingMove::GetDirection() const
{
	return mDirection;
}
void WrappingMove::Update(float deltaTime)
{
	Vector2 pos = GetOwner()->GetPosition();

	pos += GetDirection() * GetForwardSpeed() * deltaTime;

	if (pos.x <= 0)
	{
		pos.x = WINDOW_WIDTH;
	}
	else if (pos.x >= WINDOW_WIDTH)
	{
		pos.x = 0;
	}
	GetOwner()->SetPosition(pos);
	SetDeltatime(deltaTime);
}
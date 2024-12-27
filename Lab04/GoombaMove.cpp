#include "GoombaMove.h"
#include "Game.h"
#include "Math.h"
#include "PlayerMove.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
class MoveComponent;
class Actor;
class Game;
class CollisionComponent;
class Goomba;
class SpriteComponent;
GoombaMove::GoombaMove(Actor* actor, CollisionComponent* colllisoncomponent, Goomba* goomba)
: MoveComponent(actor)
{
	mDeltatime = 0.0f;
	SetForwardSpeed(FORWARD_SPEED);
	SetDirection(Vector2(-1.0f, 0.0f));
	SetOffset(Vector2::Zero);
	SetCollisionComponent(colllisoncomponent);
	SetGoomba(goomba);
}

GoombaMove::~GoombaMove()
{
}
void GoombaMove::SetDirection(const Vector2& direction)
{
	mDirection = direction;
}

Vector2 GoombaMove::GetDirection() const
{
	return mDirection;
}
void GoombaMove::Update(float deltaTime)
{
	AnimatedSprite* mGoombaSpriteComponent = GetGoomba()->GetSpriteComponent();
	mGoombaSpriteComponent->SetAnimation("walk");
	std::vector<class Block*> mBlocksCopy = GetGame()->GetBlocks();
	Vector2 offset = Vector2::Zero;
	for (Block* b : mBlocksCopy)
	{
		if (GetCollisionComponent()->Intersect(b->GetCollisionComponent()))
		{
			CollSide collSide = GetCollisionComponent()->GetMinOverlap(b->GetCollisionComponent(),
																	   mOffset);
			if (collSide == CollSide::Top && GetYSpeed() > 0.0f)
			{
				SetInAir(false);
				SetYSpeed(0.0f);
			}
			if (collSide == CollSide::Bottom && GetYSpeed() < 0.0f)
			{
				SetYSpeed(0.0f);
			}
			else if (collSide != CollSide::None)
			{
				offset += GetOffset();
			}
			if (collSide == CollSide::Right || collSide == CollSide::Left)
			{
				SetForwardSpeed(-GetForwardSpeed());
			}
		}
	}
	std::vector<class Goomba*> mGoombasCopy = GetGame()->GetGoombas();
	for (Goomba* b : mGoombasCopy)
	{
		if (b->GetCollisionComponent() != GetCollisionComponent())
		{
			if (GetCollisionComponent()->Intersect(b->GetCollisionComponent()))
			{
				CollSide collSide =
					GetCollisionComponent()->GetMinOverlap(b->GetCollisionComponent(), mOffset);
				if (collSide == CollSide::Right || collSide == CollSide::Left)
				{
					SetForwardSpeed(-GetForwardSpeed());
				}
			}
		}
	}

	Vector2 mPosition = GetOwner()->GetPosition() +
						Vector2(GetForwardSpeed() * deltaTime, GetYSpeed() * deltaTime) + offset;
	if (mPosition.y >= WINDOW_HEIGHT)
	{
		GetOwner()->SetState(ActorState::Destroy);
	}
	GetOwner()->SetPosition(mPosition);
	SetYSpeed(GetYSpeed() + GRAVITY_SPEED * deltaTime);
	if (GetGoomba()->GetStomp())
	{
		mGoombaSpriteComponent->SetAnimation("dead");
		mGoombaSpriteComponent->SetTexture(
			GetGoomba()->GetGame()->GetTexture("Assets/Goomba/Dead.png"));
		SetForwardSpeed(0.0f);
		SetStompedTime(GetStompedTime() + deltaTime);
		if (!mIsUpdate)
		{
			PlayerMove* mPlayerMove = GetGame()->GetPlayerMovement();
			Mix_Chunk* stompSound = GetGame()->GetSound("Assets/Sounds/Stomp.wav");
			Mix_PlayChannel(-1, stompSound, 0);
			mPlayerMove->SetYSpeed(HALF_JUMP);
			mIsUpdate = true;
			mPlayerMove->SetInAir(true);
		}
		if (GetStompedTime() >= DESTROY_TIME)
		{
			GetOwner()->SetState(ActorState::Destroy);
		}
	}
}

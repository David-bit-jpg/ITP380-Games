#include "PlayerMove.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
class MoveComponent;
class Actor;
class Game;
class CollisionComponent;
PlayerMove::PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent)
: MoveComponent(actor)
{
	mDeltatime = 0.0f;
	mForwardSpeed = 0.0f;
	SetDirection(Vector2::Zero);
	SetOffset(Vector2::Zero);
	SetCollisionComponent(colllisoncomponent);
	AddToLastFrame(SDL_SCANCODE_A, false);
	AddToLastFrame(SDL_SCANCODE_D, false);
	AddToLastFrame(SDL_SCANCODE_SPACE, false);
	SetInAir(true);
}

PlayerMove::~PlayerMove()
{
}
void PlayerMove::SetDirection(const Vector2& direction)
{
	mDirection = direction;
}

Vector2 PlayerMove::GetDirection() const
{
	return mDirection;
}
void PlayerMove::Update(float deltaTime)
{
	Vector2 mOwnerPos = GetOwner()->GetPosition();
	AnimatedSprite* mPlayerSpriteComponent = GetGame()->GetPlayer()->GetSpriteComponent();
	UpdatePlayerNormalAnimation(mPlayerSpriteComponent);
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
			else if (collSide == CollSide::Bottom && GetYSpeed() < 0.0f)
			{
				Mix_Chunk* bumpSound = GetGame()->GetSound("Assets/Sounds/Bump.wav");
				Mix_PlayChannel(-1, bumpSound, 0);
				SetYSpeed(0.0f);
			}
			if (collSide != CollSide::None)
			{
				offset += GetOffset();
			}
		}
	}
	SDL_Log("OFFSET X = %f, Y = %f", offset.x, offset.y);
	std::vector<class Goomba*> mGoombasCopy = GetGame()->GetGoombas();
	for (Goomba* b : mGoombasCopy)
	{
		if (GetCollisionComponent()->Intersect(b->GetCollisionComponent()))
		{
			CollSide collSide = GetCollisionComponent()->GetMinOverlap(b->GetCollisionComponent(),
																	   mOffset);
			if (collSide == CollSide::Top && !b->GetStomp())
			{
				b->SetStomp(true);
			}
			else if ((collSide == CollSide::Left || collSide == CollSide::Right) && GetInAir() &&
					 !b->GetStomp())
			{
				b->SetStomp(true);
			}
			else if (!b->GetStomp())
			{
				mPlayerSpriteComponent->SetTexture(
					GetGame()->GetPlayer()->GetGame()->GetTexture("Assets/Mario/Dead.png"));
				mPlayerSpriteComponent->SetAnimation("Dead");
				Mix_HaltChannel(GetGame()->GetBackgroundChannel());
				Mix_Chunk* deadSound = GetGame()->GetSound("Assets/Sounds/Dead.wav");
				Mix_PlayChannel(-1, deadSound, 0);
				GetOwner()->SetState(ActorState::Paused);
			}
		}
	}
	Vector2 mPosition = mOwnerPos +
						Vector2(GetForwardSpeed() * deltaTime, GetYSpeed() * deltaTime) + offset;
	if (mPosition.y > WINDOW_HEIGHT)
	{
		Mix_HaltChannel(GetGame()->GetBackgroundChannel());
		Mix_Chunk* deadSound = GetGame()->GetSound("Assets/Sounds/Dead.wav");
		Mix_PlayChannel(-1, deadSound, 0);
		if (mPosition.y > WINDOW_HEIGHT + FALL_FACTOR)
			GetOwner()->SetState(ActorState::Paused);
	}
	GetOwner()->SetPosition(mPosition);
	SetYSpeed(GetYSpeed() + GRAVITY_SPEED * deltaTime);
	Vector2 pos = GetGame()->GetCameraPos();
	if (mOwnerPos.x < -GetGame()->GetCameraPos().x)
	{
		GetOwner()->SetPosition(Vector2(-GetGame()->GetCameraPos().x, mOwnerPos.y));
	}

	if (GetGame()->GetCameraPos().x > 0)
	{
		GetGame()->SetCameraPos(Vector2(0.0f, GetGame()->GetCameraPos().y));
	}

	if (GetDirection().x == 1)
	{
		if (mOwnerPos.x + GetGame()->GetCameraPos().x >= WINDOW_WIDTH / 2)
		{
			GetGame()->SetCameraPos(Vector2(-mOwnerPos.x + WINDOW_WIDTH / 2, 0.0f));
		}
	}
	if (GetDirection().x == -1)
	{
		GetGame()->SetCameraPos(pos);
	}
	if (mOwnerPos.x >= WIN_X)
	{
		GetOwner()->SetPosition(Vector2(WIN_X, mOwnerPos.y));
		Mix_HaltChannel(GetGame()->GetBackgroundChannel());
		Mix_Chunk* deadSound = GetGame()->GetSound("Assets/Sounds/StageClear.wav");
		Mix_PlayChannel(-1, deadSound, 0);
		GetOwner()->SetState(ActorState::Paused);
	}
}
void PlayerMove::ProcessInput(const Uint8* keyState)
{
	SetDirection(Vector2::Zero);
	float forwardspeed = 0;
	if (keyState[SDL_SCANCODE_A])
	{
		SetDirection(Vector2(-1.0f, 0.0f));
		forwardspeed -= FORWARD_SPEED;
		mLastFrame[SDL_SCANCODE_A] = true;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		SetDirection(Vector2(1.0f, 0.0f));
		forwardspeed += FORWARD_SPEED;
		mLastFrame[SDL_SCANCODE_D] = true;
	}
	if (keyState[SDL_SCANCODE_SPACE] && !GetInAir() && !mLastFrame[SDL_SCANCODE_SPACE])
	{
		SetInAir(true);
		SetYSpeed(-JUMP_SPEED);
		Mix_Chunk* jumpSound = GetGame()->GetSound("Assets/Sounds/Jump.wav");
		Mix_PlayChannel(-1, jumpSound, 0);
		mLastFrame[SDL_SCANCODE_SPACE] = true;
	}
	SetForwardSpeed(forwardspeed);
	if (!keyState[SDL_SCANCODE_A])
	{
		mLastFrame[SDL_SCANCODE_A] = false;
	}
	if (!keyState[SDL_SCANCODE_D])
	{
		mLastFrame[SDL_SCANCODE_D] = false;
	}
	if (!keyState[SDL_SCANCODE_SPACE])
	{
		mLastFrame[SDL_SCANCODE_SPACE] = false;
	}
}
void PlayerMove::UpdatePlayerNormalAnimation(AnimatedSprite* mPlayerSpriteComponent) const
{
	if (GetDirection().x == 1 && !GetInAir())
	{
		mPlayerSpriteComponent->SetAnimation("RunRight");
	}
	else if (GetDirection().x == -1 && !GetInAir())
	{
		mPlayerSpriteComponent->SetAnimation("RunLeft");
	}
	else if (!GetInAir())
	{
		mPlayerSpriteComponent->SetAnimation("Idle");
	}
	if (GetDirection().x == 1 && GetInAir())
	{
		mPlayerSpriteComponent->SetAnimation("JumpRight");
	}
	else if (GetDirection().x == -1 && GetInAir())
	{
		mPlayerSpriteComponent->SetAnimation("JumpLeft");
	}
	else if (GetInAir())
	{
		std::string currentAnim = mPlayerSpriteComponent->GetAnimName();
		if (currentAnim == "RunRight" || currentAnim == "JumpRight" || currentAnim == "Idle")
		{
			mPlayerSpriteComponent->SetAnimation("JumpRight");
		}
		else
		{
			mPlayerSpriteComponent->SetAnimation("JumpLeft");
		}
	}
}
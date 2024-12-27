#include "PlayerMove.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Sword.h"
class Sword;
class MoveComponent;
class Actor;
class Collider;
class Game;
class CollisionComponent;
class AnimatedSprite;
class EnemyComponent;
PlayerMove::PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent)
: MoveComponent(actor)
, mPlayer(static_cast<Player*>(actor))
{
	mDeltatime = 0.0f;
	mForwardSpeed = 0.0f;
	Sword* sword = new Sword(GetGame());
	mSword = sword;
	SetFaceDirection(Direction::None);
	SetOffset(Vector2::Zero);
	SetCollisionComponent(colllisoncomponent);
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
void PlayerMove::SetFaceDirection(const PlayerMove::Direction& facedirection)
{
	mFaceDirection = facedirection;
}

PlayerMove::Direction PlayerMove::GetFaceDirection() const
{
	return mFaceDirection;
}
void PlayerMove::Update(float deltaTime)
{
	if (mIsAttacking) //if it's attacking, it should not move
	{
		UpdateAnimation();
		mAttackTimer += deltaTime; //update timer
		if (mAttackTimer >= ATTACK_DURATION)
		{
			mIsAttacking = false;
			mAttackTimer = 0.0f;
		}
		for (EnemyComponent* ec :
			 GetGame()->GetEnemyComponents()) //if collide with any enemy components
		{
			if (GetSword()->GetCollisionComponent()->Intersect(ec->GetCollisionComponent()))
			{
				ec->TakeDamage();
			}
		}
	}
	else
	{
		mOffset = Vector2::Zero; //check for collision
		for (Collider* c : GetGame()->GetColliders())
		{
			if (GetCollisionComponent()->Intersect(c->GetCollisionComponent()))
			{
				GetCollisionComponent()->GetMinOverlap(c->GetCollisionComponent(), mOffset);
			}
		}
		for (EnemyComponent* ec : GetGame()->GetEnemyComponents())
		{
			if (GetCollisionComponent()->Intersect(ec->GetCollisionComponent()))
			{
				GetCollisionComponent()->GetMinOverlap(ec->GetCollisionComponent(), mOffset);
			}
		}
		GetOwner()->SetPosition(GetOwner()->GetPosition() + mDirection * PLAYER_SPEED * deltaTime +
								mOffset);
	}
	const Vector2 CAMERA_OFFSET(CAMERA_OFFSETX, CAMERA_OFFSETY);
	Vector2 playerPos = GetOwner()->GetPosition();
	GetGame()->SetCameraPos(playerPos + CAMERA_OFFSET); //update camera pos
}
void PlayerMove::ProcessInput(const Uint8* keyState)
{
	UpdateAnimation();
	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed &&
		!mIsAttacking) //if press space, and space is not been pressed, and it's not in attacking state
	{
		mSpacePressed = true;
		mIsAttacking = true;
		mIsAttacking = true;
		mAttackTimer = 0.0f;
		GetGame()->GetAudio()->PlaySound("SwordSlash.wav");
		mPlayer->GetSpriteComponent()->ResetAnimTimer();
	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
	mDirection = Vector2(0.0f, 0.0f);
	mIsMoving = false;
	if (keyState[SDL_SCANCODE_W])
	{
		mFaceDirection = Direction::Up;
		mDirection = Vector2(0.0f, -1.0f);
		mIsMoving = true;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		mFaceDirection = Direction::Down;
		mDirection = Vector2(0.0f, 1.0f);
		mIsMoving = true;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		mFaceDirection = Direction::Left;
		mDirection = Vector2(-1.0f, 0.0f);
		mIsMoving = true;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		mFaceDirection = Direction::Right;
		mDirection = Vector2(1.0f, 0.0f);
		mIsMoving = true;
	}
}
void PlayerMove::UpdateAnimation()
{
	AnimatedSprite* mPlayerAnimatedSprite = mPlayer->GetSpriteComponent();
	if (!mIsAttacking) //different stand/walk animations
	{
		if (mDirection.x == 0.0f && mDirection.y == 0.0f)
		{
			switch (mFaceDirection)
			{
			case Up:
				mPlayerAnimatedSprite->SetAnimation("StandUp");
				break;
			case Down:
				mPlayerAnimatedSprite->SetAnimation("StandDown");
				break;
			case Left:
				mPlayerAnimatedSprite->SetAnimation("StandLeft");
				break;
			case Right:
				mPlayerAnimatedSprite->SetAnimation("StandRight");
				break;
			case None:
				break;
			}
		}
		else
			switch (mFaceDirection)
			{
			case Up:
				mPlayerAnimatedSprite->SetAnimation("WalkUp");
				break;
			case Down:
				mPlayerAnimatedSprite->SetAnimation("WalkDown");
				break;
			case Left:
				mPlayerAnimatedSprite->SetAnimation("WalkLeft");
				break;
			case Right:
				mPlayerAnimatedSprite->SetAnimation("WalkRight");
				break;
			case None:
				break;
			}
	}
	else
	{
		switch (mFaceDirection) //different attack animations
		{
		case Up:
			mPlayerAnimatedSprite->SetAnimation("AttackUp");
			GetSword()->SetPosition(mPlayer->GetPosition() + UP_OFFSET);
			GetSword()->GetCollisionComponent()->SetSize(SWORD_SIZE_ONE, SWORD_SIZE_TWO);
			break;
		case Down:
			mPlayerAnimatedSprite->SetAnimation("AttackDown");
			GetSword()->SetPosition(mPlayer->GetPosition() + DOWN_OFFSET);
			GetSword()->GetCollisionComponent()->SetSize(SWORD_SIZE_ONE, SWORD_SIZE_TWO);
			break;
		case Left:
			mPlayerAnimatedSprite->SetAnimation("AttackLeft");
			GetSword()->SetPosition(mPlayer->GetPosition() + LEFT_OFFSET);
			GetSword()->GetCollisionComponent()->SetSize(SWORD_SIZE_TWO, SWORD_SIZE_ONE);
			break;
		case Right:
			mPlayerAnimatedSprite->SetAnimation("AttackRight");
			GetSword()->SetPosition(mPlayer->GetPosition() + RIGHT_OFFSET);
			GetSword()->GetCollisionComponent()->SetSize(SWORD_SIZE_TWO, SWORD_SIZE_ONE);
			break;
		case None:
			break;
		}
	}
}
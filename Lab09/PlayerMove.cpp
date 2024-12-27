#include "PlayerMove.h"
#include "Game.h"
#include "Actor.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Player.h"
#include "Renderer.h"
#include "Random.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Prop.h"
#include "PortalGun.h"
#include "Crosshair.h"
#include "PlayerMesh.h"
#include "SegmentCast.h"
PlayerMove::PlayerMove(Actor* actor)
: MoveComponent(actor)
{
	ChangeState(MoveState::Falling);
	Crosshair* crosshair = new Crosshair(actor);
	mCrosshair = crosshair;
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Update(float deltaTime)
{
	switch (mCurrentState)
	{
	case MoveState::OnGround:
		PlayerMove::UpdateOnGround(deltaTime);
		break;
	case MoveState::Jump:
		PlayerMove::UpdateJump(deltaTime);
		break;
	case MoveState::Falling:
		PlayerMove::UpdateFalling(deltaTime);
		break;
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{
	bool hasGun = dynamic_cast<Player*>(mOwner)->HasGun();
	bool leftMousePressed = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
	bool rightMousePressed = mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	bool rPressed = keyState[SDL_SCANCODE_R];
	bool spacePressed = keyState[SDL_SCANCODE_SPACE]; //current state
	if (keyState[SDL_SCANCODE_W])
	{
		AddForce(GetOwner()->GetForward() * FORCE);
	}
	if (keyState[SDL_SCANCODE_S])
	{
		AddForce(GetOwner()->GetForward() * -FORCE);
	}

	if (keyState[SDL_SCANCODE_D])
	{
		AddForce(GetOwner()->GetRight() * FORCE);
	}
	if (keyState[SDL_SCANCODE_A])
	{
		AddForce(GetOwner()->GetRight() * -FORCE);
	}
	if (spacePressed && mCurrentState == MoveState::OnGround && !mSpacePressed) //last state
	{
		AddForce(mJumpForce);
		ChangeState(MoveState::Jump);
	}
	if (leftMousePressed && !mLeftMousePressed && hasGun)
	{
		CreatePortal(true);
	}
	if (rightMousePressed && !mRightMousePressed && hasGun)
	{
		CreatePortal(false);
	}
	if (rPressed && !mRPressed)
	{
		Portal* currentBlue = GetGame()->GetBluePortal();
		Portal* currentOrange = GetGame()->GetOrangePortal();
		if (currentBlue)
		{
			currentBlue->SetState(ActorState::Destroy);
			GetGame()->SetBluePortal(nullptr);
		}
		if (currentOrange)
		{
			currentOrange->SetState(ActorState::Destroy);
			GetGame()->SetOrangePortal(nullptr);
		}
		mCrosshair->SetState(CrosshairState::Default);
	}
	mSpacePressed = spacePressed; //set last state to current state
	mRPressed = rPressed;
	mLeftMousePressed = leftMousePressed;
	mRightMousePressed = rightMousePressed;
	float angularSpeed = relativeMouse.x / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);
	float pitchSpeed = relativeMouse.y / 500.0f * Math::Pi * 10.0f;
	mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(pitchSpeed);
}
void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);
	CollSide collside = CollSide::None;
	bool isLanded = false;
	for (Actor* a : GetGame()->GetColliders())
	{
		if (a && a->GetComponent<CollisionComponent>())
		{
			collside = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									a->GetComponent<CollisionComponent>());
			if (collside == CollSide::Top)
			{
				isLanded = true;
			}
		}
	}
	if (!isLanded)
	{
		ChangeState(MoveState::Falling);
	}
}
void PlayerMove::UpdateJump(float deltaTime)
{
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollSide collside = CollSide::None;
	for (Actor* a : GetGame()->GetColliders())
	{
		if (a && a->GetComponent<CollisionComponent>())
		{
			collside = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									a->GetComponent<CollisionComponent>());
			if (collside == CollSide::Bottom)
			{
				mVelocity.z = 0.0f;
			}
		}
	}
	if (mVelocity.z <= 0.0f)
	{
		ChangeState(MoveState::Falling);
	}
}
void PlayerMove::UpdateFalling(float deltaTime)
{
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollSide collside = CollSide::None;
	bool isLanded = false;
	for (Actor* a : GetGame()->GetColliders())
	{
		if (a && a->GetComponent<CollisionComponent>())
		{
			collside = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									a->GetComponent<CollisionComponent>());
			if (collside == CollSide::Top && mVelocity.z <= 0.0f)
			{
				isLanded = true;
			}
		}
	}
	if (isLanded)
	{
		mVelocity.z = 0.0f;
		ChangeState(MoveState::OnGround);
	}
}
void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();
	if (mVelocity.z <= MIN_Z)
	{
		mVelocity.z = MIN_Z;
	}
	GetOwner()->SetPosition(GetOwner()->GetPosition() + mVelocity * deltaTime);
	float mRotation = GetOwner()->GetRotation() + mAngularSpeed * deltaTime;
	GetOwner()->SetRotation(mRotation);
	mPendingForces = Vector3::Zero;
}
void PlayerMove::FixXYVelocity()
{
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
	if (xyVelocity.Length() >= MAX_SPEED)
	{
		xyVelocity.Normalize();
		xyVelocity *= MAX_SPEED;
	}
	if (mCurrentState == MoveState::OnGround)
	{
		if (((mAcceleration.x > 0 && xyVelocity.x < 0) ||
			 (mAcceleration.x < 0 && xyVelocity.x > 0)) ||
			Math::NearlyZero(mAcceleration.x))
		{
			xyVelocity.x *= STOPCOEE;
		}
		if (((mAcceleration.y > 0 && xyVelocity.y < 0) ||
			 (mAcceleration.y < 0 && xyVelocity.y > 0)) ||
			Math::NearlyZero(mAcceleration.y))
		{
			xyVelocity.y *= STOPCOEE;
		}
	}
	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}
CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* collider)
{
	Vector3 offside;
	CollSide collside = self->GetMinOverlap(collider, offside);
	if (collside != CollSide::None)
	{
		GetOwner()->SetPosition(GetOwner()->GetPosition() + offside);
	}
	return collside;
}
void PlayerMove::CollectGun(Actor* gun)
{
	if (!dynamic_cast<Player*>(mOwner)->HasGun())
	{
		dynamic_cast<Player*>(mOwner)->SetGun(true);
		gun->SetState(ActorState::Destroy);
		new PlayerMesh(GetGame());
	}
}
void PlayerMove::CreatePortal(bool isBlue)
{
	Vector3 nearPlane = GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 0.0f));
	Vector3 farPlane = GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 1.0f));
	Vector3 direction = farPlane - nearPlane;
	direction.Normalize();
	LineSegment ls = LineSegment(nearPlane, nearPlane + LINE_LEN * direction);
	CastInfo ci;
	if (SegmentCast(GetGame()->GetColliders(), ls, ci))
	{
		Vector3 normal = ci.mNormal;
		Vector3 defaultV = Vector3::UnitX;

		Vector3 rotationAxis = Vector3::Cross(defaultV, normal);
		rotationAxis.Normalize();

		Quaternion quat;

		float dot = Vector3::Dot(normal, defaultV);
		float angle = Math::Acos(dot);
		if (dot == 1.0f)
		{
			quat = Quaternion::Identity;
		}
		else if (dot == -1.0f)
		{
			quat = Quaternion(Vector3::UnitZ, Math::Pi);
		}
		else
		{
			quat = Quaternion(rotationAxis, angle);
		}
		Block* block = dynamic_cast<Block*>(ci.mActor);
		if (block)
		{
			Portal* p = new Portal(GetGame(), isBlue);
			p->SetPosition(ci.mPoint);
			if (isBlue)
			{
				Portal* current = GetGame()->GetBluePortal();
				if (current)
				{
					current->SetState(ActorState::Destroy);
				}
				GetGame()->SetBluePortal(p);
			}
			else
			{
				Portal* current = GetGame()->GetOrangePortal();
				if (current)
				{
					current->SetState(ActorState::Destroy);
				}
				GetGame()->SetOrangePortal(p);
			}
			Portal* currentOrange = GetGame()->GetOrangePortal();
			Portal* currentBlue = GetGame()->GetBluePortal();
			if (currentOrange && !currentBlue)
			{
				mCrosshair->SetState(CrosshairState::OrangeFill);
			}
			else if (currentBlue && !currentOrange)
			{
				mCrosshair->SetState(CrosshairState::BlueFill);
			}
			else if (currentBlue && currentOrange)
			{
				mCrosshair->SetState(CrosshairState::BothFill);
			}
			p->SetQuat(quat);
		}
	}
}
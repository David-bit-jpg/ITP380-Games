#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* actor)
: Component(actor, 50)
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	float mRotation = GetOwner()->GetRotation() + mAngularSpeed * deltaTime;
	GetOwner()->SetRotation(mRotation);

	Vector3 mForward = GetOwner()->GetForward();
	Vector3 mPosition = GetOwner()->GetPosition() + mForward * mForwardSpeed * deltaTime;
	GetOwner()->SetPosition(mPosition);

	Vector3 mRight = GetOwner()->GetRight();
	Vector3 mPositionRight = GetOwner()->GetPosition() + mRight * mStrafeSpeed * deltaTime;
	GetOwner()->SetPosition(mPositionRight);
}

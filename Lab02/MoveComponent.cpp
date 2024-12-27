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

	Vector2 mForward = GetOwner()->GetForward();
	Vector2 mPosition = GetOwner()->GetPosition() + mForward * mForwardSpeed * deltaTime;
	GetOwner()->SetPosition(mPosition);
}

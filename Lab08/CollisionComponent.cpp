#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
, mDepth(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	Vector3 thisMin = GetMin();
	Vector3 thisMax = GetMax();
	Vector3 otherMin = other->GetMin();
	Vector3 otherMax = other->GetMax();

	bool noIntersection = thisMax.x < otherMin.x || thisMax.y < otherMin.y ||
						  thisMax.z < otherMin.z || otherMax.x < thisMin.x ||
						  otherMax.y < thisMin.y || otherMax.z < thisMin.z;

	return !noIntersection;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale().x / 2.0f;
	v.y -= mWidth * mOwner->GetScale().y / 2.0f;
	v.z -= mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale().x / 2.0f;
	v.y += mWidth * mOwner->GetScale().y / 2.0f;
	v.z += mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector3& offset) const
{
	offset = Vector3::Zero;
	// TODO: Implement
	return CollSide::None;
}

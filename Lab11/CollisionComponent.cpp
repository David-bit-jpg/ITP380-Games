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
	// offset = 需要移动来离开other的距离
	offset = Vector3::Zero;
	if (Intersect(other))
	{
		float rightDist = other->GetMax().y - GetMin().y;
		float leftDist = GetMax().y - other->GetMin().y;
		float bottomDist = GetMax().z - other->GetMin().z;
		float topDist = other->GetMax().z - GetMin().z;
		float backDist = GetMax().x - other->GetMin().x;
		float frontDist = other->GetMax().x - GetMin().x;
		float minOverlap = std::min({fabs(leftDist), fabs(rightDist), fabs(topDist),
									 fabs(bottomDist), fabs(frontDist), fabs(backDist)});
		if (minOverlap == fabs(leftDist))
		{
			offset = Vector3(0.0f, -leftDist, 0.0f);
			return CollSide::Left;
		}
		if (minOverlap == fabs(rightDist))
		{
			offset = Vector3(0.0f, rightDist, 0.0f);
			return CollSide::Right;
		}
		if (minOverlap == fabs(topDist))
		{
			offset = Vector3(0.0f, 0.0f, topDist);
			return CollSide::Top;
		}
		if (minOverlap == fabs(bottomDist))
		{
			offset = Vector3(0.0f, 0.0f, -bottomDist);
			return CollSide::Bottom;
		}
		if (minOverlap == fabs(frontDist))
		{
			offset = Vector3(frontDist, 0.0f, 0.0f);
			return CollSide::Front;
		}
		if (minOverlap == fabs(backDist))
		{
			offset = Vector3(-backDist, 0.0f, 0.0f);
			return CollSide::Back;
		}
	}
	return CollSide::None;
}

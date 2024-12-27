#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	if (GetMin().x <= other->GetMax().x && GetMax().x >= other->GetMin().x)
	{
		if (GetMin().y <= other->GetMax().y && GetMax().y >= other->GetMin().y)
		{
			return true;
		}
	}
	return false;
}

Vector2 CollisionComponent::GetMin() const
{
	return Vector2(mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f,
				   mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f);
}

Vector2 CollisionComponent::GetMax() const
{
	return Vector2(mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f,
				   mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	offset = Vector2::Zero;
	if (Intersect(other))
	{
		float leftDist = other->GetMax().x - GetMin().x;
		float rightDist = GetMax().x - other->GetMin().x;
		float topDist = other->GetMax().y - GetMin().y;
		float bottomDist = GetMax().y - other->GetMin().y;
		float minOverlap =
			std::min({fabs(leftDist), fabs(rightDist), fabs(topDist), fabs(bottomDist)});
		if (minOverlap == fabs(leftDist))
		{
			offset = Vector2(leftDist, 0.0f);
			return CollSide::Right;
		}
		if (minOverlap == fabs(rightDist))
		{
			offset = Vector2(-rightDist, 0.0f);
			return CollSide::Left;
		}
		if (minOverlap == fabs(topDist))
		{
			offset = Vector2(0.0f, topDist);
			return CollSide::Bottom;
		}
		if (minOverlap == fabs(bottomDist))
		{
			offset = Vector2(0.0f, -bottomDist);
			return CollSide::Top;
		}
	}
	return CollSide::None;
}

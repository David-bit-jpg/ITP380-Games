#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"

enum class CollSide
{
	None,
	Top,
	Bottom,
	Left,
	Right
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(class Actor* owner);
	~CollisionComponent();

	// Set width/height of this box
	void SetSize(float width, float height)
	{
		mWidth = width;
		mHeight = height;
	}

	// Returns true if this box intersects with other
	bool Intersect(const CollisionComponent* other) const;

	// Get min and max points of box
	Vector2 GetMin() const;
	Vector2 GetMax() const;

	// Get width, height, center of box
	const Vector2& GetCenter() const;
	float GetWidth() const { return mWidth; }
	float GetHeight() const { return mHeight; }
	// Returns side of minimum overlap against other
	// or None if no overlap
	// Takes in by reference the offset to fix
	// "this" so it no longer overlaps with "other"
	CollSide GetMinOverlap(const CollisionComponent* other, Vector2& offset) const;

private:
	float mWidth;
	float mHeight;
};

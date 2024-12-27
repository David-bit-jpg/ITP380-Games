#pragma once
#include "Component.h"
#include "Math.h"

enum class CollSide
{
	None,
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(class Actor* owner);
	~CollisionComponent();

	// Set width/height of this box
	void SetSize(float width, float height, float depth)
	{
		mWidth = width;
		mHeight = height;
		mDepth = depth;
	}

	// Returns true if this box intersects with other
	bool Intersect(const CollisionComponent* other) const;

	// Get min and max points of box
	Vector3 GetMin() const;
	Vector3 GetMax() const;

	// Get width, height, center of box
	Vector3 GetCenter() const;
	float GetWidth() const { return mWidth; }
	float GetHeight() const { return mHeight; }
	float GetDepth() const { return mDepth; }

	// Returns side of minimum overlap against other
	// or None if no overlap
	// Takes in by reference the offset to fix
	// "this" so it no longer overlaps with "other"
	CollSide GetMinOverlap(const CollisionComponent* other, Vector3& offset) const;

private:
	float mWidth;
	float mHeight;
	float mDepth;
};

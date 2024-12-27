#pragma once
#include "Math.h"
#include <vector>

struct LineSegment
{
	LineSegment() {}
	LineSegment(const Vector3& start, const Vector3& end);
	// Get point along segment where 0 <= t <= 1
	Vector3 PointOnSegment(float t) const;
	float Length() const;

	Vector3 mStart;
	Vector3 mEnd;
};

// Used to give helpful information about cast results, if there's an intersection
struct CastInfo
{
	// Point of collision
	Vector3 mPoint;
	// Normal at collision
	Vector3 mNormal;
	// Owning actor you collided with
	class Actor* mActor = nullptr;
};

// Returns true if the segment intersects with any of the Actors in the vector,
// in which case outInfo is populated with the relevant information
bool SegmentCast(const std::vector<class Actor*>& actors, const LineSegment& l, CastInfo& outInfo,
				 Actor* ignoreActor = nullptr);

// Returns true if the segment intersects with the specified actor, in which case
// outInfo is populated with information about the closest actor that intersects
bool SegmentCast(class Actor* actor, const LineSegment& l, CastInfo& outInfo);

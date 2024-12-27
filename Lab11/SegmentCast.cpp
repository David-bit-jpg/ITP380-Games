#include "SegmentCast.h"
#include <algorithm>
#include "CollisionComponent.h"
#include "Actor.h"

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
: mStart(start)
, mEnd(end)
{
}

Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

float LineSegment::Length() const
{
	Vector3 diff = mEnd - mStart;
	return diff.Length();
}

// Use this to fix jittering of contains
bool FuzzyLessThan(float a, float b)
{
	return ((b - a) > 0.01f);
}

// Tests whether point is inside a box
bool Contains(const Vector3& min, const Vector3& max, const Vector3& point)
{
	bool outside = FuzzyLessThan(point.x, min.x) || FuzzyLessThan(point.y, min.y) ||
				   FuzzyLessThan(point.z, min.z) || FuzzyLessThan(max.x, point.x) ||
				   FuzzyLessThan(max.y, point.y) || FuzzyLessThan(max.z, point.z);
	// If none of these are true, the point is inside the box
	return !outside;
}

// Helper function for Intersect
bool TestSidePlane(float start, float end, float negd, const Vector3& norm,
				   std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::NearlyZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		// Test that t is within bounds
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& l, const CollisionComponent* cc, float& outT, Vector3& outNorm)
{
	// Vector to save all possible t values, and normals for those sides
	static std::vector<std::pair<float, Vector3>> tValues;
	tValues.clear();

	Vector3 min = cc->GetMin();
	Vector3 max = cc->GetMax();

	// If the segment starts in the box, then return t = 0 and no normal
	if (Contains(min, max, l.mStart))
	{
		outT = 0.0f;
		outNorm = Vector3::Zero;
		return true;
	}

	// Test the x planes
	TestSidePlane(l.mStart.x, l.mEnd.x, min.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, max.x, Vector3::UnitX, tValues);
	// Test the y planes
	TestSidePlane(l.mStart.y, l.mEnd.y, min.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, max.y, Vector3::UnitY, tValues);
	// Test the z planes
	TestSidePlane(l.mStart.z, l.mEnd.z, min.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, max.z, Vector3::UnitZ, tValues);

	// Sort the t values in ascending order
	std::sort(tValues.begin(), tValues.end(),
			  [](const std::pair<float, Vector3>& a, const std::pair<float, Vector3>& b) {
				  return a.first < b.first;
			  });
	// Test if the box contains any of these points of intersection
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (Contains(min, max, point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	// None of the intersections are within bounds of box
	return false;
}

bool SegmentCast(const std::vector<class Actor*>& actors, const LineSegment& l, CastInfo& outInfo,
				 Actor* ignoreActor)
{
	bool collided = false;
	// Initialize closestT to infinity, so first
	// intersection will always update closestT
	float closestT = Math::Infinity;
	Vector3 norm;
	// Test against all boxes
	for (auto a : actors)
	{
		if (a == ignoreActor)
		{
			continue;
		}

		CollisionComponent* cc = a->GetComponent<CollisionComponent>();
		if (cc)
		{
			float t = Math::Infinity;
			// Does the segment intersect with the box?
			if (Intersect(l, cc, t, norm))
			{
				// Is this closer than previous intersection?
				if (t < closestT)
				{
					closestT = t;
					outInfo.mPoint = l.PointOnSegment(t);
					outInfo.mNormal = norm;
					outInfo.mActor = a;
					collided = true;
				}
			}
		}
	}
	return collided;
}

bool SegmentCast(Actor* actor, const LineSegment& l, CastInfo& outInfo)
{
	bool collided = false;
	Vector3 norm;
	// Test against all boxes
	CollisionComponent* cc = actor->GetComponent<CollisionComponent>();

	if (cc)
	{
		float t = Math::Infinity;
		// Does the segment intersect with the box?
		if (Intersect(l, cc, t, norm))
		{
			// Is this closer than previous intersection?
			outInfo.mPoint = l.PointOnSegment(t);
			outInfo.mNormal = norm;
			outInfo.mActor = actor;
			collided = true;
		}
	}

	return collided;
}

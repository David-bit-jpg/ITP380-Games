#pragma once
#include "Math.h"
#include "Component.h"
#include "MeshComponent.h"
#include "SegmentCast.h"
#include "Portal.h"
#include <vector>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	~LaserComponent();
	void SetIgnore(Actor* a) { mActorToIgnore = a; }
	Actor* GetLastHitActor() const { return mLastHitActor; }
	void SetDisabled(bool a) { mIsDisabled = a; }

private:
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	Matrix4 TransformWorldMatrix(LineSegment ls);

	std::vector<LineSegment> mLineSegments;
	Actor* mActorToIgnore = nullptr;
	class Actor* mLastHitActor = nullptr;
	bool mIsDisabled = false;

	float const LASER_LEN = 350.0f;
	float const SECOND_POS = 5.5f;
};

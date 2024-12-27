#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	void SnapToIdeal();

protected:
	void Update(float deltaTime) override;
	Vector3 GetIdealPos();

	Vector3 mCamPos;
	Vector3 mVelocity;

	const float HDIST = 60.0f;
	const float TARGETDIST = 50.0f;
	const float VDIST = 70.0f;
	const float HORIZONTAL_DISTANCE = 60.0f;
	const float TARGET_OFFSET = 50.0f;
	const float SPRING_CONSTANT = 256.0f;
	const float DAMP_CONSTANT = 2.0f * Math::Sqrt(SPRING_CONSTANT);
	const float IDEAL_POS = 70.0f;
};

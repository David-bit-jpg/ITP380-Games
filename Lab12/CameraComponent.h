#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	float GetPitchAngle() const { return mPitchAngle; }
	void SetPitchAngle(float a) { mPitchAngle = a; }

	float GetPitchSpeed() const { return mPitchSpeed; }
	void SetPitchSpeed(float a) { mPitchSpeed = a; }
	Vector3 GetCamForward() const { return mForward; }

protected:
	void Update(float deltaTime) override;

	Vector3 mForward;
	const float TARGETDIST = 50.0f;
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
};

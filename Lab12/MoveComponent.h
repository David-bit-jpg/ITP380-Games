#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* actor);

	// Update the move component
	void Update(float deltaTime) override;

	// Getters/setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }

protected:
	// Angular speed (in radians/second)
	float mAngularSpeed;
	// Forward speed (in pixels/second)
	float mForwardSpeed;
	float mStrafeSpeed = 0.0f;
};

#pragma once
#include "VehicleMove.h"
#include <cstddef>
#include "Math.h"
class EnemyMove : public VehicleMove
{
public:
	EnemyMove(class Actor* owner);
	~EnemyMove();
	void Update(float deltaTime) override;

protected:
	int mIndex = 1;

	float const ENE_MIN_ACC = 500.0f;
	float const ENE_MAX_ACC = 2500.0f;
	float const ENE_RAMP_TIME = 1.0f;
	float const ENE_ANGULAR_ACC = 5.0f * Math::Pi;

	float const LINEDRAG_COFF_PRESSED = 0.9f;
	float const LINEDRAG_COFF_NOTPRESSED = 0.975f;
	float const ANGULAR_COFF = 0.9f;
	const float FALLSPEED = 10.0f;
	const float TARGETZ = -100.0f;
};

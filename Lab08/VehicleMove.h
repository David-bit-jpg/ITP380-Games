#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"
#include <vector>
class VehicleMove : public Component
{
public:
	enum class TurnDirection
	{
		None,
		Left,
		Right
	};
	VehicleMove(class Actor* owner);
	~VehicleMove();

	void Update(float deltaTime) override;

	TurnDirection GetTurnDirection() const { return mTurnDirection; }
	void SetTurnDirection(TurnDirection direction) { mTurnDirection = direction; }
	void SetPedalPressed(bool pressed) { mAccPressed = pressed; }
	float GetAngularV() const { return mAngularVelocity; }
	int GetLap() const { return mCurrentLap; }
	int GetCheckPoint() const { return mLastCheckPoint; }
	int GetCurrentCheckPoint() const { return mCheckIndex; }
	virtual void OnLapChange(int newLap) {}
	float GetDistanceToNext();
	std::vector<Vector4> GetAllCheckPoints() const { return mCheckPoints; }

	void SetMinAcc(float f) { mMinAcc = f; }
	void SetMaxAcc(float f) { mMaxAcc = f; }
	void SetRampTime(float f) { mRampTime = f; }
	void SetAngularAcc(float f) { mAngularAcc = f; }
	void SetLinDragPressed(float f) { mLinDragPressed = f; }
	void SetLinDragNotPressed(float f) { mLinDragNotPressed = f; }
	void SetAngularCoff(float f) { mAngularCoff = f; }
	void SetFallSpeed(float f) { mFallSpeed = f; }
	void SetTargetZ(float f) { mTargetZ = f; }

protected:
	float mAngularVelocity = 0.0f;
	bool mAccPressed = false;
	Vector3 mVelocity = Vector3::Zero;
	float mAccTime = 0.0f;
	TurnDirection mTurnDirection = TurnDirection::None;
	float mTurnFactor = 1.0f;
	bool mIsEnemy = false;
	std::vector<Vector4> mCheckPoints;
	int mCol = 0;
	int mRow = 0;
	int mCurrentLap = 0;
	int mLastCheckPoint = -1;
	int mCheckIndex = 0;
	float mTimer = 0.0f;
	bool mIsSoundPlayed = false;

	Vector4 FindNearestCheckPoint();

	float mMinAcc = 1000.0f;
	float mMaxAcc = 2500.0f;
	float mRampTime = 2.0f;
	float mAngularAcc = 5.0f * Math::Pi;
	float mLinDragPressed = 0.9f;
	float mLinDragNotPressed = 0.975f;
	float mAngularCoff = 0.9f;
	float mFallSpeed = 10.0f;
	float mTargetZ = -100.0f;
};

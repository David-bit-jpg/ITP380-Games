#include "VehicleMove.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "HeightMap.h"
#include "CSVHelper.h"
#include <fstream>
#include <iostream>
class HeightMap;
VehicleMove::VehicleMove(Actor* owner)
: Component(owner, 50)
{
	std::ifstream file("Assets/HeightMap/Checkpoints.csv");
	std::string line;
	std::vector<Vector4> mTemp;
	std::getline(file, line);
	while (!file.eof())
	{
		mRow++;
		std::getline(file, line);
		if (!line.empty())
		{
			std::vector<std::string> mLineInfo = CSVHelper::Split(line);
			if (!mLineInfo.empty())
			{
				mCol++;
				Vector4 v = Vector4(std::stoi(mLineInfo[1]), std::stoi(mLineInfo[2]),
									std::stoi(mLineInfo[3]), std::stoi(mLineInfo[4]));
				mTemp.push_back(v);
			}
		}
	}
	mRow -= 1;
	mCol = mCol / mRow;
	mCheckPoints = mTemp;
}

VehicleMove::~VehicleMove()
{
}

void VehicleMove::Update(float deltaTime)
{
	HeightMap* mTemp = GetGame()->GetHeightMap();

	if (mAccPressed)
	{
		mAccTime += deltaTime;
		float mLerpFactor = mAccTime / mRampTime;
		if (mLerpFactor >= 1.0f)
		{
			mLerpFactor = 1.0f;
		}
		float mAccMag = Math::Lerp(mMinAcc, mMaxAcc, mLerpFactor);
		mVelocity += mOwner->GetForward() * mAccMag * deltaTime;
		mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
		mVelocity *= mLinDragPressed;
	}
	else
	{
		mAccTime = 0.0f;
		mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
		mVelocity *= mLinDragNotPressed;
	}
	mTurnFactor = 0.0f;
	if (mTemp->IsOnTrack(Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y)))
	{
		float mZMeg = Math::Lerp(
			mOwner->GetPosition().z,
			mTemp->GetHeight(Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y)), 0.1f);
		mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, mZMeg));
	}

	if (mTurnDirection != TurnDirection::None)
	{
		if (mTurnDirection == TurnDirection::Left)
		{
			mTurnFactor = -1.0f;
		}
		else if (mTurnDirection == TurnDirection::Right)
		{
			mTurnFactor = 1.0f;
		}
	}
	mAngularVelocity += mAngularAcc * mTurnFactor * deltaTime;
	mOwner->SetRotation(mOwner->GetRotation() + mAngularVelocity * deltaTime);
	mAngularVelocity *= mAngularCoff;
	Vector2 m2DPos = Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y);
	Vector2 mCellPos = mTemp->WorldToCell(m2DPos);
	Vector4 v = mCheckPoints[mCheckIndex];
	int mCurrentX = static_cast<int>(mCellPos.x);
	int mCurrentY = static_cast<int>(mCellPos.y);
	int mCheckMinX = static_cast<int>(v.x);
	int mCheckMaxX = static_cast<int>(v.y);
	int mCheckMinY = static_cast<int>(v.z);
	int mCheckMaxY = static_cast<int>(v.w);
	if (mCurrentX >= mCheckMinX && mCurrentX <= mCheckMaxX && mCurrentY >= mCheckMinY &&
		mCurrentY <= mCheckMaxY)
	{
		mLastCheckPoint = mCheckIndex;
		mCheckIndex++;
		if (mCheckIndex == 1 && mLastCheckPoint == 0)
		{
			mCurrentLap++;
			OnLapChange(mCurrentLap);
		}
	}
	if (mCheckIndex == mCheckPoints.size())
	{
		mCheckIndex = 0;
	}
}

float VehicleMove::GetDistanceToNext()
{
	HeightMap* mTemp = GetGame()->GetHeightMap();
	Vector4 v = mCheckPoints[mCheckIndex];
	Vector3 mNextPos = mTemp->CellToWorld(static_cast<int>((v.x + v.y) / 2),
										  static_cast<int>((v.z + v.w) / 2));
	Vector2 mPlayerPos = Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y);
	return Vector2::Distance(mPlayerPos, Vector2(mNextPos.x, mNextPos.y));
}

Vector4 VehicleMove::FindNearestCheckPoint()
{
	HeightMap* mTemp = GetGame()->GetHeightMap();
	float minDistance = std::numeric_limits<float>::max();
	Vector3 vehiclePos = mOwner->GetPosition();
	Vector4 nearest;
	for (const Vector4& v : mCheckPoints)
	{
		Vector3 checkpointPos = mTemp->CellToWorld(static_cast<int>((v.x + v.y) / 2),
												   static_cast<int>((v.z + v.w) / 2));
		float distance = Vector3::Distance(vehiclePos, checkpointPos);
		if (distance < minDistance)
		{
			nearest = v;
			minDistance = distance;
		}
	}
	return nearest;
}

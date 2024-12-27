#include "EnemyMove.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
EnemyMove::EnemyMove(Actor* owner)
: VehicleMove(owner)
{
	SetMinAcc(ENE_MIN_ACC);
	SetMaxAcc(ENE_MAX_ACC);
	SetRampTime(ENE_RAMP_TIME);
	SetAngularAcc(ENE_ANGULAR_ACC);
	SetLinDragPressed(LINEDRAG_COFF_PRESSED);
	SetLinDragNotPressed(LINEDRAG_COFF_NOTPRESSED);
	SetAngularCoff(ANGULAR_COFF);
	SetFallSpeed(FALLSPEED);
	SetTargetZ(TARGETZ);
}

EnemyMove::~EnemyMove()
{
}

void EnemyMove::Update(float deltaTime)
{
	Vector3 mNext = GetGame()->GetEnemy()->GetNode()[mIndex];
	Vector3 mToTarget = mNext - mOwner->GetPosition();
	mToTarget.Normalize();
	Vector3 mForward = mOwner->GetForward();
	float dot = Vector3::Dot(mForward, mToTarget);
	float distance = Vector3::Distance(mNext, mOwner->GetPosition());
	if (dot >= 0.99f)
	{
		VehicleMove::SetPedalPressed(true);
	}
	else
	{
		VehicleMove::SetPedalPressed(false);
	}
	Vector3 cross = Vector3::Cross(mForward, mToTarget); //means sin
	if (cross.z > 0)
	{
		VehicleMove::SetPedalPressed(true);
		VehicleMove::SetTurnDirection(TurnDirection::Right);
	}
	else if (cross.z < 0)
	{
		VehicleMove::SetPedalPressed(true);
		VehicleMove::SetTurnDirection(TurnDirection::Left);
	}
	else
	{
		VehicleMove::SetPedalPressed(true);
		VehicleMove::SetTurnDirection(TurnDirection::None);
	}
	if (distance <= 180.0f)
	{
		mIndex++;
		if (mIndex >= GetGame()->GetEnemy()->GetNode().size())
		{
			mIndex = 0;
		}
	}

	VehicleMove::Update(deltaTime);
}

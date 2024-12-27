#pragma once
#include "Actor.h"
class LaserComponent;
class CollisionComponent;
class MeshComponent;
enum class TurretState
{
	Idle,
	Search,
	Priming,
	Firing,
	Falling,
	Dead
};
class TurretHead : public Actor
{
public:
	TurretHead(class Game* game, Actor* parent);
	~TurretHead();
	void ChangeState(TurretState ts)
	{
		mTurretState = ts;
		mStateTimer = 0.0f;
	}
	void Die();

private:
	Actor* mParent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	Actor* mLaser = nullptr;
	Actor* mAquired = nullptr;
	LaserComponent* mLaserComponent = nullptr;
	TurretState mTurretState = TurretState::Idle;
	float mStateTimer = 0.0f;
	Quaternion mQuat = Quaternion::Identity;
	Quaternion mTempQuat;
	float mInterTimer = 0.0f;
	float mTeleportTimer = 0.25f;
	Vector3 mFallVelocity = Vector3::Zero;
	float mFireCoolDown = 0.0f;

	void OnUpdate(float deltaTime) override;
	void UpdateIdle(float deltaTime);
	void UpdateSearch(float deltaTime);
	void UpdatePriming(float deltaTime);
	void UpdateFiring(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateDead(float deltaTime);
	bool CheckAquired();
	bool CalculateTeleport();

	float const SCALE = 0.75f;
	float const PRIMING_TIME = 1.5f;
	float const SIDEDIST = 75.0f;
	float const UPDIST = 25.0f;
	float const FWDDIST = 200.0f;
	float const DURATION = 0.5f;
	float const SEARCH_TIME = 5.0f;
	float const TELEPORT_INT = 0.25f;
	float const FALLMAG = 25.0f;
	float const FALL_LIMIT = 800.0f;
	float const DIE_OFFSET = 15.0f;
	float const FIRE_INT = 0.05f;
	float const FIRE_DAMAGE = 2.5f;
	Vector3 const GRAVITY = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 const POSITION_HEAD = Vector3(0.0f, 0.0f, 18.75f);
	Vector3 const POSITION_LASER = Vector3(0.0f, 0.0f, 10.0f);
};

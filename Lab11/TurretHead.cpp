#include "TurretHead.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"
#include "Random.h"
#include "LaserComponent.h"
#include "HealthComponent.h"
#include "TurretBase.h"

TurretHead::TurretHead(class Game* game, Actor* parent)
: Actor(game, parent)
{
	mParent = parent;
	Actor* laser = new Actor(game, this);
	laser->SetPosition(mParent->GetPosition() + POSITION_LASER);
	mLaser = laser;
	LaserComponent* lc = new LaserComponent(mLaser);
	lc->SetIgnore(mParent);
	mLaserComponent = lc;
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/TurretHead.gpmesh"));
	mMeshComponent = mc;
	SetPosition(POSITION_HEAD * Vector3::UnitZ);
}
TurretHead::~TurretHead()
{
}
void TurretHead::OnUpdate(float deltaTime)
{
	mTeleportTimer += deltaTime;
	mStateTimer += deltaTime;
	mFireCoolDown -= deltaTime;
	switch (mTurretState)
	{
	case TurretState::Idle:
		UpdateIdle(deltaTime);
		break;
	case TurretState::Search:
		UpdateSearch(deltaTime);
		break;
	case TurretState::Priming:
		UpdatePriming(deltaTime);
		break;
	case TurretState::Firing:
		UpdateFiring(deltaTime);
		break;
	case TurretState::Falling:
		UpdateFalling(deltaTime);
		break;
	case TurretState::Dead:
		UpdateDead(deltaTime);
		break;
	default:
		break;
	}
}
void TurretHead::UpdateIdle(float deltaTime)
{
	if (CalculateTeleport())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Falling;
	}
	else if (CheckAquired())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Priming;
	}
}
void TurretHead::UpdateSearch(float deltaTime)
{
	if (CalculateTeleport())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Falling;
	}
	else if (CheckAquired())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Priming;
	}
	else if (mStateTimer >= SEARCH_TIME)
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Idle;
	}
	if (mInterTimer == 0.0f)
	{
		Vector3 turretHeadPos = GetPosition();
		Vector3 forward = Vector3::UnitX;
		Vector3 center = turretHeadPos + forward * FWDDIST;
		float randomAngle = Random::GetFloatRange(-Math::Pi, Math::Pi);
		Vector3 offset = Vector3(0.0f, SIDEDIST * Math::Cos(randomAngle),
								 UPDIST * Math::Sin(randomAngle));
		Vector3 point = center + offset;
		Vector3 toPoint = point - turretHeadPos;
		toPoint.Normalize();
		Vector3 normal = toPoint;
		Vector3 defaultV = Vector3::UnitX;
		Vector3 rotationAxis = Vector3::Cross(defaultV, normal);
		rotationAxis.Normalize();

		float dot = Vector3::Dot(normal, defaultV);
		float angle = Math::Acos(dot);
		if (dot == 1.0f)
		{
			mTempQuat = Quaternion::Identity;
		}
		else if (dot == -1.0f)
		{
			mTempQuat = Quaternion(Vector3::UnitZ, Math::Pi);
		}
		else
		{
			mTempQuat = Quaternion(rotationAxis, angle);
		}
	}
	mInterTimer += deltaTime;
	Quaternion mTargetQuat;
	if (mInterTimer <= DURATION)
	{
		mQuat = Quaternion::Slerp(Quaternion::Identity, mTempQuat, mInterTimer / DURATION);
		mTargetQuat = mTempQuat;
	}
	else if (mInterTimer <= DURATION * 2)
	{
		mQuat = Quaternion::Slerp(mTempQuat, Quaternion::Identity,
								  (mInterTimer - DURATION) / DURATION);
		mTargetQuat = Quaternion::Identity;
	}
	else if (mInterTimer > DURATION * 2)
	{
		mInterTimer = 0.0f;
		mQuat = mTargetQuat;
	}
	SetQuat(mQuat);
}
void TurretHead::UpdatePriming(float deltaTime)
{
	if (CalculateTeleport())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Falling;
	}
	else if (mLaserComponent->GetLastHitActor() == mAquired)
	{
		if (mStateTimer >= PRIMING_TIME)
		{
			mStateTimer = 0.0f;
			mFireCoolDown = FIRE_INT;
			mTurretState = TurretState::Firing;
		}
	}
	else
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Search;
	}
}
void TurretHead::UpdateFiring(float deltaTime)
{
	if (CalculateTeleport())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Falling;
	}
	else if (mLaserComponent->GetLastHitActor() == mAquired &&
			 !mLaserComponent->GetLastHitActor()->GetComponent<HealthComponent>()->IsDead())
	{
		if (mFireCoolDown <= 0.0f)
		{
			mFireCoolDown = FIRE_INT;
			mAquired->GetComponent<HealthComponent>()->TakeDamage(FIRE_DAMAGE, GetWorldPosition());
		}
	}
	else
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Search;
	}
}
void TurretHead::UpdateFalling(float deltaTime)
{
	Vector3 offset;
	mParent->SetPosition(mParent->GetPosition() + mFallVelocity * deltaTime);
	if (!CalculateTeleport())
	{
		mFallVelocity += GRAVITY * deltaTime;
		for (Actor* c : GetGame()->GetColliders())
		{
			if (c != mParent && c->GetComponent<CollisionComponent>())
			{
				if (mParent->GetComponent<CollisionComponent>()->Intersect(
						c->GetComponent<CollisionComponent>()))
				{
					CollSide collside = mParent->GetComponent<CollisionComponent>()->GetMinOverlap(
						c->GetComponent<CollisionComponent>(), offset);
					if (collside != CollSide::None)
					{
						mParent->SetPosition(mParent->GetPosition() + offset);
					}
					if (collside == CollSide::Top && mFallVelocity.z < 0.0f)
					{
						mParent->SetPosition(mParent->GetPosition() -
											 Vector3(0.0f, 0.0f, DIE_OFFSET));
						Die();
						TurretBase* tb = dynamic_cast<TurretBase*>(c);
						if (tb)
						{
							mParent->SetPosition(
								mParent->GetPosition() -
								Vector3(0.0f, 0.0f,
										tb->GetComponent<CollisionComponent>()->GetHeight() / 2));
							tb->Die();
						}
					}
				}
			}
		}
	}
	if (mFallVelocity.Length() >= FALL_LIMIT)
	{
		mFallVelocity.Normalize();
		mFallVelocity *= FALL_LIMIT;
	}
}
void TurretHead::UpdateDead(float deltaTime)
{
	if (CalculateTeleport())
	{
		mStateTimer = 0.0f;
		mTurretState = TurretState::Falling;
	}
}
bool TurretHead::CheckAquired()
{
	Actor* lastComponent = mLaserComponent->GetLastHitActor();
	if (lastComponent)
	{
		if (lastComponent->GetComponent<HealthComponent>() &&
			!lastComponent->GetComponent<HealthComponent>()->IsDead())
		{
			mAquired = lastComponent;
			return true;
		}
	}
	return false;
}
bool TurretHead::CalculateTeleport()
{
	if (mTeleportTimer >= TELEPORT_INT && GetGame()->GetBluePortal() &&
		GetGame()->GetOrangePortal())
	{
		if (mParent->GetComponent<CollisionComponent>()->Intersect(
				GetGame()->GetBluePortal()->GetComponent<CollisionComponent>()))
		{
			mParent->SetPosition(GetGame()->GetOrangePortal()->GetPosition());
			mTeleportTimer = 0.0f;
			mFallVelocity += FALLMAG * GetGame()->GetOrangePortal()->GetQuatForward();
			return true;
		}
		else if (mParent->GetComponent<CollisionComponent>()->Intersect(
					 GetGame()->GetOrangePortal()->GetComponent<CollisionComponent>()))
		{
			mParent->SetPosition(GetGame()->GetBluePortal()->GetPosition());
			mTeleportTimer = 0.0f;
			mFallVelocity += FALLMAG * GetGame()->GetBluePortal()->GetQuatForward();
			return true;
		}
	}
	return false;
}
void TurretHead::Die()
{
	Quaternion quat = Quaternion(Vector3::UnitX, Math::PiOver2);
	mParent->SetQuat(quat);
	mLaserComponent->SetDisabled(true);
	mStateTimer = 0.0f;
	mTurretState = TurretState::Dead;
}
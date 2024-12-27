#pragma once
#include "Actor.h"

class HealthComponent;
class TurretHead;
class CollisionComponent;
class MeshComponent;
class TurretBase : public Actor
{
public:
	TurretBase(class Game* game);
	~TurretBase();

	void Die();

private:
	MeshComponent* mMeshComponent = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	class TurretHead* mHead = nullptr;
	HealthComponent* mHealthComponent = nullptr;

	float const SCALE = 0.75f;
	float const CC_XZ = 25.0f;
	float const CC_Y = 110.0f;
};

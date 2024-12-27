#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class CollisionComponent;
class MeshComponent;
class EnergyLauncher : public Actor
{
public:
	EnergyLauncher(class Game* game);
	~EnergyLauncher();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	void SetDoor(std::string d) { mDoor = d; }
	void SetCoolDown(float f) { mCoolDown = f; }

private:
	void OnUpdate(float deltaTime) override;
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	float const CC_SIZE = 50.0f;
	float mCoolDown = 1.5f;
	std::string mDoor;
	float const DISPLACEMENT = 20.0f;
	float mTimer = 0.0f;
	bool mStop = false;
};

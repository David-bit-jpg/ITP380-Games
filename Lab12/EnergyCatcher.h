#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class CollisionComponent;
class MeshComponent;
class EnergyCatcher : public Actor
{
public:
	EnergyCatcher(class Game* game);
	~EnergyCatcher();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	void SetDoor(std::string d) { mDoor = d; }
	bool GetActivate() const { return mIsActivated; }
	void SetActivate(bool b) { mIsActivated = b; }

private:
	void OnUpdate(float deltaTime) override;
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	float const CC_SIZE = 50.0f;
	std::string mDoor;
	bool mIsActivated = false;
};

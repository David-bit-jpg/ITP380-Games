#pragma once
#include "Actor.h"

class CollisionComponent;
class MeshComponent;
class EnergyGlass : public Actor
{
public:
	EnergyGlass(class Game* game);
	~EnergyGlass();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	float const CC_SIZE = 1.0f;
};

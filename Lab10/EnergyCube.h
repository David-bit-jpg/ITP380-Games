#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class CollisionComponent;
class MeshComponent;
class EnergyCube : public Actor
{
public:
	EnergyCube(class Game* game);
	~EnergyCube();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	MeshComponent* GetMeshComponent() const { return mMeshComponent; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	float const CC_SIZE = 25.0f;
};

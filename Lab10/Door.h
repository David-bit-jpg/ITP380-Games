#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class CollisionComponent;
class MeshComponent;
class Door : public Actor
{
public:
	Door(class Game* game, std::string name);
	~Door();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	std::string GetName() const { return mName; }
	void SetOpened(bool m) { mIsOpened = m; }
	bool GetOpened() const { return mIsOpened; }

private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	std::string mName;
	bool mIsOpened = false;
};

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
	bool mSoundPlayed = false;

private:
	void OnUpdate(float deltaTime) override;
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;

	std::string mName;
	Actor* mLeftHalf;
	Actor* mRightHalf;

	bool mIsOpened = false;
	float mOpenTimer = 0.0f;

	float const OPEN_TIME = 1.0f;
	float const FINAL_Y = 100.0f;
};

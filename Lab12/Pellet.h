#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Portal.h"
class CollisionComponent;
class MeshComponent;
class Portal;
class Pellet : public Actor
{
public:
	Pellet(class Game* game, Vector3 dir, Vector3 pos);
	~Pellet();
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	void SetDirection(Vector3 v) { mDirection = v; }

private:
	void OnUpdate(float deltaTime) override;
	void CalculateTeleport(Portal* entryPort, Portal* exitPort);

	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;

	Vector3 mDirection;
	float mTimer = 0.0f;
	float mSpeed = 500.0f;
	Vector3 mVelocity;
	bool mIsGreen = false;

	float const THRESHOLD = 0.25f;
	float const UPVALUE = 50.0f;
	float const CATCHER_DISPLACEMENT = 40.0f;
	float const CC_SIZE = 25.0f;
	float const DAMAGE = 100.0f;
};

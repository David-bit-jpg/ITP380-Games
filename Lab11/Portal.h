#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
class CollisionComponent;
class Portal : public Actor
{
public:
	Portal(class Game* game, bool isBlue);
	Vector3 GetPortalOutVector(const Vector3& init, Portal* exitPortal, float wComponent);
	void SetCollisionComponent(CollisionComponent* cc) { mCollisionComponent = cc; }
	CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }

private:
	void CalcViewMatrix(struct PortalData& portalData, Portal* exitPortal);
	void OnUpdate(float deltaTime) override;
	CollisionComponent* mCollisionComponent = nullptr;
	float const TARGETDIST = 50.0f;
	bool mIsBlue = false;
};

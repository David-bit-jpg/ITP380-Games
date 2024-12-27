#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
class MeshComponent;
class CollisionComponent;
class Game;
class PortalGun : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;
	void OnUpdate(float deltaTime) override;

	float const CC_X = 8.0f;
	float const CC_Y = 8.0f;
	float const CC_Z = 8.0f;

public:
	PortalGun(Game* game);
	~PortalGun();

	CollisionComponent* GetPortalGunCollisionComponent() const { return mCollisionComponent; }
	MeshComponent* GetMeshComponent() const { return mMeshComponent; }
};

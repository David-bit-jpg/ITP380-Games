#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"

class MeshComponent;
class CollisionComponent;
class Game;
class Prop : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	MeshComponent* mMeshComponent = nullptr;

public:
	Prop(Game* game);
	~Prop();

	MeshComponent* GetMeshComponent() const { return mMeshComponent; }
	void SetMeshComponent(MeshComponent* mc) { mMeshComponent = mc; }
	void SetCollisionComponent(CollisionComponent* mc) { mCollisionComponent = mc; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
};

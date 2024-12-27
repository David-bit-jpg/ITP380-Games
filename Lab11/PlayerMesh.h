#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
class MeshComponent;
class Game;
class PlayerMesh : public Actor
{
private:
	MeshComponent* mMeshComponent = nullptr;
	Vector3 const SCALE = Vector3(1.0f, 2.5f, 2.5f);
	Vector3 const UNPROJECT = Vector3(300.0f, -250.0f, 0.4f);

	void OnUpdate(float deltaTime) override;

public:
	PlayerMesh(Game* game);
	~PlayerMesh();

	MeshComponent* GetMeshComponent() const { return mMeshComponent; }
};

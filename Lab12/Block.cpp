#include "Actor.h"
#include "SDL2/SDL.h"
#include "Block.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
Block::Block(Game* game)
: Actor(game)
{
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Cube.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_X, CC_Y, CC_Z);
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
}

Block::~Block()
{
	GetGame()->RemoveColliders(this);
}
#include "EnergyCube.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

EnergyCube::EnergyCube(class Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/EnergyCube.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_SIZE, CC_SIZE, CC_SIZE);
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
}
EnergyCube::~EnergyCube()
{
	GetGame()->RemoveColliders(this);
}
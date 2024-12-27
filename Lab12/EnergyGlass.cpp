#include "EnergyGlass.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

EnergyGlass::EnergyGlass(class Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, true);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Cube.gpmesh"));
	mc->SetTextureIndex(17);
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_SIZE, CC_SIZE, CC_SIZE);
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
}
EnergyGlass::~EnergyGlass()
{
	GetGame()->RemoveColliders(this);
}
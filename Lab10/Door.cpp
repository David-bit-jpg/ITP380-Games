#include "Door.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

Door::Door(class Game* game, std::string name)
: Actor(game)
{
	mName = name;
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorFrame.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorFrame.gpmesh")->GetWidth(),
				GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorFrame.gpmesh")->GetHeight(),
				GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorFrame.gpmesh")->GetDepth());
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
	GetGame()->AddDoors(this);
}
Door::~Door()
{
	GetGame()->RemoveColliders(this);
	GetGame()->RemoveDoors(this);
}
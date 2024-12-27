#include "EnergyCatcher.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

EnergyCatcher::EnergyCatcher(class Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/EnergyCatcher.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_SIZE, CC_SIZE, CC_SIZE);
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
}
EnergyCatcher::~EnergyCatcher()
{
	GetGame()->RemoveColliders(this);
}
void EnergyCatcher::OnUpdate(float deltaTime)
{
	if (!mDoor.empty())
	{
		for (Door* d : GetGame()->GetDoors())
		{
			if (d->GetName() == mDoor && !d->GetOpened() && mIsActivated)
			{
				d->SetOpened(true);
				GetGame()->RemoveColliders(d);
			}
		}
	}
}

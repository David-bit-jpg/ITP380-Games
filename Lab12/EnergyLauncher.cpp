#include "EnergyLauncher.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"
#include "Pellet.h"
#include "Door.h"
EnergyLauncher::EnergyLauncher(class Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/EnergyLauncher.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_SIZE, CC_SIZE, CC_SIZE);
	mCollisionComponent = cc;
	GetGame()->AddColliders(this);
}
EnergyLauncher::~EnergyLauncher()
{
	GetGame()->RemoveColliders(this);
}

void EnergyLauncher::OnUpdate(float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer >= mCoolDown && !mStop)
	{
		GetGame()->GetAudio()->PlaySound("PelletFire.ogg", false, this);
		new Pellet(GetGame(), GetForward(), GetPosition() + GetForward() * DISPLACEMENT);
		mTimer = 0.0f;
	}
	if (!mDoor.empty())
	{
		for (Door* d : GetGame()->GetDoors())
		{
			if (d->GetName() == mDoor && d->GetOpened())
			{
				mStop = true;
			}
		}
	}
}

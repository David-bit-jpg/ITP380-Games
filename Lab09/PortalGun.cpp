#include "Actor.h"
#include "SDL2/SDL.h"
#include "PortalGun.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
PortalGun::PortalGun(Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/PortalGun.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_X, CC_Y, CC_Z);
	mCollisionComponent = cc;
	GetGame()->SetGun(this);
}

PortalGun::~PortalGun()
{
}
void PortalGun::OnUpdate(float deltaTime)
{
	if (mCollisionComponent->Intersect(GetGame()->GetPlayer()->GetPlayerCollisionComponent()))
	{
		GetGame()->GetPlayer()->GetPlayerMove()->CollectGun(this);
	}
	SetRotation(GetRotation() + Math::Pi * deltaTime);
}
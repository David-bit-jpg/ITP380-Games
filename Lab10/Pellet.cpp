#include "Pellet.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"
#include "Portal.h"
#include "EnergyCatcher.h"
#include "EnergyCube.h"
#include "EnergyGlass.h"

Pellet::Pellet(class Game* game, Vector3 dir, Vector3 pos)
: Actor(game)
{
	SetPosition(pos);
	mDirection = dir;
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Sphere.gpmesh"));
	mc->SetTextureIndex(1);
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_SIZE, CC_SIZE, CC_SIZE);
	mCollisionComponent = cc;
	mVelocity = mSpeed * mDirection;
}
Pellet::~Pellet()
{
}
void Pellet::OnUpdate(float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer >= THRESHOLD && GetGame()->GetBluePortal() && GetGame()->GetOrangePortal() &&
		mCollisionComponent->Intersect(
			GetGame()->GetBluePortal()->GetComponent<CollisionComponent>()))
	{
		CalculateTeleport(GetGame()->GetBluePortal(), GetGame()->GetOrangePortal());
		mTimer = 0.0f;
	}
	if (mTimer >= THRESHOLD && GetGame()->GetBluePortal() && GetGame()->GetOrangePortal() &&
		mCollisionComponent->Intersect(
			GetGame()->GetOrangePortal()->GetComponent<CollisionComponent>()))
	{
		CalculateTeleport(GetGame()->GetOrangePortal(), GetGame()->GetBluePortal());
		mTimer = 0.0f;
	}
	if (mTimer >= THRESHOLD && GetGame()->GetPlayer() &&
		mCollisionComponent->Intersect(GetGame()->GetPlayer()->GetComponent<CollisionComponent>()))
	{
		SetState(ActorState::Destroy);
	}
	for (Actor* c : GetGame()->GetColliders())
	{
		if (mTimer >= THRESHOLD && c->GetComponent<CollisionComponent>() &&
			mCollisionComponent->Intersect(c->GetComponent<CollisionComponent>()))
		{
			EnergyCatcher* ec = dynamic_cast<EnergyCatcher*>(c);
			EnergyCube* ecu = dynamic_cast<EnergyCube*>(c);
			EnergyGlass* eg = dynamic_cast<EnergyGlass*>(c);
			if (ec && !ec->GetActivate())
			{
				SetPosition(ec->GetPosition() + ec->GetForward() * CATCHER_DISPLACEMENT);
				mVelocity = Vector3::Zero;
				ec->SetActivate(true);
			}
			else if (ecu)
			{
				mMeshComponent->SetTextureIndex(2);
				mIsGreen = true;
			}
			else if (eg && mIsGreen)
			{
				continue;
			}
			else
			{
				SetState(ActorState::Destroy);
			}
		}
	}
	SetPosition(GetPosition() + deltaTime * mVelocity);
}
void Pellet::CalculateTeleport(Portal* entryPort, Portal* exitPort)
{
	//velocity
	Vector3 velocity = mVelocity;
	Vector3 newD;
	if (exitPort->GetQuatForward().z == 1.0f || exitPort->GetQuatForward().z == -1.0f ||
		entryPort->GetQuatForward().z == 1.0f || entryPort->GetQuatForward().z == -1.0f)
	{
		newD = exitPort->GetQuatForward();
	}
	else
	{
		velocity.Normalize();
		newD = entryPort->GetPortalOutVector(velocity, exitPort, 0.0f);
	}
	mVelocity = newD * mSpeed;

	//pos
	Vector3 newPos;
	if (exitPort->GetQuatForward().z == 1.0f || exitPort->GetQuatForward().z == -1.0f ||
		entryPort->GetQuatForward().z == 1.0f || entryPort->GetQuatForward().z == -1.0f)
	{
		newPos = exitPort->GetPosition();
	}
	else
	{
		newPos = entryPort->GetPortalOutVector(GetPosition(), exitPort, 1.0f);
	}
	SetPosition(newPos + UPVALUE * exitPort->GetQuatForward());
}
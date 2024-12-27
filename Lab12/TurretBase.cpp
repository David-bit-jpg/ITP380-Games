#include "TurretBase.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"
#include "TurretHead.h"
#include "HealthComponent.h"

TurretBase::TurretBase(class Game* game)
: Actor(game)
{
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/TurretBase.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_XZ, CC_Y, CC_XZ);
	mCollisionComponent = cc;
	TurretHead* head = new TurretHead(game, this);
	mHead = head;
	HealthComponent* hc = new HealthComponent(this);
	hc->SetOnDeathCallback([this]() {
		Die();
	});
	hc->SetOnDamageCallback([this](const Vector3& location) {
		mHead->TakeDamage();
	});
	mHealthComponent = hc;
	GetGame()->AddColliders(this);
}
TurretBase::~TurretBase()
{
	GetGame()->RemoveColliders(this);
}
void TurretBase::Die()
{
	if (mHead)
	{
		mHead->Die();
	}
}
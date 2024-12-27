#include "Door.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

Door::Door(class Game* game, std::string name)
: Actor(game)
{
	Actor* mLeft = new Actor(game, this);
	MeshComponent* mcL = new MeshComponent(mLeft);
	mcL->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorLeft.gpmesh"));
	Actor* mRight = new Actor(game, this);
	MeshComponent* mcR = new MeshComponent(mRight);
	mcR->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/DoorRight.gpmesh"));
	mLeftHalf = mLeft;
	mRightHalf = mRight;
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
void Door::OnUpdate(float deltaTime)
{
	if (mIsOpened && mOpenTimer <= OPEN_TIME)
	{
		if (!mSoundPlayed)
		{
			GetGame()->GetAudio()->PlaySound("DoorOpen.ogg", false, this);
			mSoundPlayed = true;
		}
		mOpenTimer += deltaTime;
		mLeftHalf->SetPosition(
			Vector3::Lerp(Vector3::Zero, Vector3(0.0f, -FINAL_Y, 0.0f), mOpenTimer));
		mRightHalf->SetPosition(
			Vector3::Lerp(Vector3::Zero, Vector3(0.0f, FINAL_Y, 0.0f), mOpenTimer));
	}
}
#include "Actor.h"
#include "SDL2/SDL.h"
#include "PlayerMesh.h"
#include "Game.h"
#include "Renderer.h"
PlayerMesh::PlayerMesh(Game* game)
: Actor(game)
{
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/PortalGun.gpmesh"));
	mMeshComponent = mc;
}

PlayerMesh::~PlayerMesh()
{
}
void PlayerMesh::OnUpdate(float deltaTime)
{
	SetPosition(GetGame()->GetRenderer()->Unproject(UNPROJECT));
	float pitchAngle = GetGame()->GetPlayer()->GetCameraComponent()->GetPitchAngle();
	Quaternion mQuatPitch = Quaternion(Vector3::UnitY, pitchAngle);
	float yawAngle = GetGame()->GetPlayer()->GetRotation();
	Quaternion mQuatYaw = Quaternion(Vector3::UnitZ, yawAngle);
	Quaternion mQuatCombine = Quaternion::Concatenate(mQuatPitch, mQuatYaw);
	SetQuat(mQuatCombine);
}
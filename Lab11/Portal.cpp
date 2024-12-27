#include "Portal.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

Portal::Portal(class Game* game, bool isBlue)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, true);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Portal.gpmesh"));
	PortalMeshComponent* pmc = new PortalMeshComponent(this);
	mIsBlue = isBlue;
	if (isBlue)
	{
		pmc->SetTextureIndex(0);
		mc->SetTextureIndex(2);
	}
	else
	{
		pmc->SetTextureIndex(1);
		mc->SetTextureIndex(3);
	}
}

Vector3 Portal::GetPortalOutVector(const Vector3& init, Portal* exitPortal, float wComponent)
{
	Matrix4 inverseEntryWorld = this->GetWorldTransform();
	inverseEntryWorld.Invert();
	Vector3 initOBJ = Vector3::Transform(init, inverseEntryWorld, wComponent);
	Matrix4 rotationMatrixZ = Matrix4::CreateRotationZ(Math::Pi);
	Vector3 initZOBJ = Vector3::Transform(initOBJ, rotationMatrixZ, wComponent);
	Vector3 initPortalView = Vector3::Transform(initZOBJ, exitPortal->GetWorldTransform(),
												wComponent);

	return initPortalView;
}
void Portal::CalcViewMatrix(struct PortalData& portalData, Portal* exitPortal)
{
	if (!exitPortal)
	{
		portalData.mView = Matrix4::CreateScale(0.0f);
		return;
	}
	Vector3 portalViewCameraPos = GetPortalOutVector(GetGame()->GetPlayer()->GetPosition(),
													 exitPortal, 1.0f);
	Vector3 portalViewCameraFwd = GetPortalOutVector(GetGame()->GetPlayer()->GetForward(),
													 exitPortal, 0.0f);
	Vector3 portalViewCameraUp = exitPortal->GetWorldTransform().GetZAxis();
	Matrix4 portalViewMatrix = Matrix4::CreateLookAt(
		portalViewCameraPos, portalViewCameraPos + portalViewCameraFwd * TARGETDIST,
		portalViewCameraUp);
	portalData.mView = portalViewMatrix;
	portalData.mCameraPos = portalViewCameraPos;
	portalData.mCameraForward = portalViewCameraFwd;
	portalData.mCameraUp = portalViewCameraUp;
}
void Portal::OnUpdate(float deltaTime)
{
	if (mIsBlue)
	{
		CalcViewMatrix(GetGame()->GetRenderer()->GetBluePortal(), GetGame()->GetOrangePortal());
	}
	else
	{
		CalcViewMatrix(GetGame()->GetRenderer()->GetOrangePortal(), GetGame()->GetBluePortal());
	}
}

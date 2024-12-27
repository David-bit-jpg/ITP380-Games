#include "CameraComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
CameraComponent::CameraComponent(Actor* owner)
: Component(owner, 50)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	float z = mCamPos.z;
	Vector3 newPos = GetOwner()->GetPosition();
	Vector3 newForward = GetOwner()->GetForward();
	Vector3 displacement = mCamPos - GetIdealPos();
	Vector3 springAcc = (-SPRING_CONSTANT * displacement) - (DAMP_CONSTANT * mVelocity);
	mVelocity += springAcc * deltaTime;
	mCamPos = mVelocity * deltaTime + mCamPos;
	mCamPos.z = z;
	Vector3 targetPos = newPos + newForward * TARGETDIST;
	Matrix4 viewMatrix = Matrix4::CreateLookAt(mCamPos, targetPos, Vector3::UnitZ);
	GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

Vector3 CameraComponent::GetIdealPos()
{
	Vector3 newPos = GetOwner()->GetPosition();
	Vector3 newForward = GetOwner()->GetForward();
	return (newPos - newForward * HDIST + Vector3(0.0f, 0.0f, IDEAL_POS));
}

void CameraComponent::SnapToIdeal()
{
	mCamPos = GetIdealPos();
	Vector3 newPos = GetOwner()->GetPosition();
	Vector3 newForward = GetOwner()->GetForward();
	Vector3 targetPos = newPos + newForward * TARGETDIST;
	Matrix4 viewMatrix = Matrix4::CreateLookAt(mCamPos, targetPos, Vector3::UnitZ);
	GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

#include "CameraComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
CameraComponent::CameraComponent(Actor* owner)
: Component(owner)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	mPitchAngle += mPitchSpeed * deltaTime;
	mPitchAngle = Math::Clamp(mPitchAngle, -Math::Pi / 2.1f, Math::Pi / 2.1f);
	Vector3 newPos = GetOwner()->GetPosition();
	Matrix4 pitchMatrix = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yawMatrix = Matrix4::CreateRotationZ(GetOwner()->GetRotation());
	Matrix4 rotationMatrix = pitchMatrix * yawMatrix;
	Vector3 forward = Vector3::Transform(Vector3::UnitX, rotationMatrix);
	mForward = forward;
	Vector3 targetPos = newPos + forward * TARGETDIST;
	Matrix4 viewMatrix = Matrix4::CreateLookAt(newPos, targetPos, Vector3::UnitZ);
	GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

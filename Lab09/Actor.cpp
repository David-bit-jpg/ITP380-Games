#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include "SDL2/SDL.h"
#include <string>
Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector3::Zero)
, mScale(Vector3(1.0f, 1.0f, 1.0f))
, mRotation(0.0f)
{
	GetGame()->AddActor(this);
}

Actor::~Actor()
{
	GetGame()->RemoveActor(this);

	for (Component* c : GetComponents())
	{
		delete c;
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	if (GetState() == ActorState::Active)
	{
		for (Component* c : GetComponents())
		{
			c->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}
	CalcWorldTransform();
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
	if (GetState() == ActorState::Active)
	{
		for (Component* c : GetComponents())
		{
			c->ProcessInput(keyState, mouseButtons, relativeMouse);
		}
		OnProcessInput(keyState, mouseButtons, relativeMouse);
	}
}

void Actor::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
const std::vector<class Component*>& Actor::GetComponents()
{
	return mComponents;
}
void Actor::CalcWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrixZ = Matrix4::CreateRotationZ(mRotation);
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);
	Matrix4 rotationMatrixX = Matrix4::CreateRotationX(mRollAngle);
	Matrix4 quaternionMatrix = Matrix4::CreateFromQuaternion(mQuat);
	SetWorldTransform(scaleMatrix * rotationMatrixZ * rotationMatrixX * quaternionMatrix *
					  translationMatrix);
}
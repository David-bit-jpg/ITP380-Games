#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
class Game;

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector2::Zero)
, mScale(1.0f)
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
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if (GetState() == ActorState::Active)
	{
		for (Component* c : GetComponents())
		{
			c->ProcessInput(keyState);
		}
		OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
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

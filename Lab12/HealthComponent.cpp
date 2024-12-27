#include "HealthComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

HealthComponent::HealthComponent(Actor* owner, float health)
: Component(owner)
{
	mHealth = health;
}

HealthComponent::~HealthComponent()
{
}
void HealthComponent::TakeDamage(float damage, const Vector3& direction)
{
	mHealth -= damage;
	if (!IsDead())
	{
		if (mOnDamage)
		{
			mOnDamage(direction);
		}
	}
	else
	{
		if (mOnDeath)
		{
			mOnDeath();
		}
	}
}
void HealthComponent::SetOnDamageCallback(std::function<void(const Vector3&)> callback)
{
	mOnDamage = callback;
}

void HealthComponent::SetOnDeathCallback(std::function<void()> callback)
{
	mOnDeath = callback;
}
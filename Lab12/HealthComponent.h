#pragma once
#include "Component.h"
#include <cstddef>
#include <functional>
class HealthComponent : public Component
{
public:
	HealthComponent(class Actor* owner, float health = 100.0f);
	~HealthComponent();
	void SetOnDamageCallback(std::function<void(const Vector3&)> callback);
	void SetOnDeathCallback(std::function<void()> callback);
	float GetHealth() const { return mHealth; }
	bool IsDead() const { return mHealth <= 0.0f; }
	void TakeDamage(float damage, const Vector3& direction);

private:
	float mHealth = 100.0f;
	std::function<void(const Vector3&)> mOnDamage;
	std::function<void()> mOnDeath;
};

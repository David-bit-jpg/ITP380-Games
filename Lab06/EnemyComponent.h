#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include "CollisionComponent.h"
#include <vector>
#include <functional>
class Game;
class Actor;
class CollisionComponent;
class EnemyComponent : public Component
{
public:
	// (Lower draw order corresponds with further back)
	EnemyComponent(class Actor* owner, int drawOrder = 100);
	~EnemyComponent();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	int GetHitPoint() const { return mHitPoint; }
	void SetHitPoint(int p) { mHitPoint = p; }
	void TakeDamage();
	void SetOnDamageCallback(std::function<void()> callback);
	void SetOnDeathCallback(std::function<void()> callback);

private:
	CollisionComponent* mCollisionComponent = nullptr;
	float mTimePassed = 0.0f;
	void Update(float deltaTime) override;
	const float THRESHOLD = 0.25f;
	int mHitPoint = 0;
	std::function<void()> mOnDamage;
	std::function<void()> mOnDeath;
};

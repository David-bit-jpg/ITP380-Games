#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"

class CollisionComponent;
class SpriteComponent;

class DeadFrog : public Actor
{
private:
	SpriteComponent* mSpriteComponent;
	const float LIFETIME_FACTOR = 0.5f;
	float mLifeTime = 0.0f;

public:
	DeadFrog(Game* game);
	~DeadFrog();
	SpriteComponent* GetSpriteComponent() const;
	void OnUpdate(float deltaTime) override;
};

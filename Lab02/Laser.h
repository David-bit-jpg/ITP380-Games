#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"

class MoveComponent;
class SpriteComponent;

class Laser : public Actor
{
private:
	SpriteComponent* mSpriteComponent;
	MoveComponent* mMoveComponent;
	const float MOVESPEED = 400.0f;
	const float MIN_DISTANCE = 70.0f;
	const float LIFETIME_FACTOR = 1.0f;
	float mLifeTime = 0.0f;

public:
	Laser(Game* game);
	~Laser();
	MoveComponent* GetMoveComponent() const;
	SpriteComponent* GetSpriteComponent() const;
	void OnUpdate(float deltaTime) override;
};

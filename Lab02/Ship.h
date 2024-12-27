#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"

class MoveComponent;
class SpriteComponent;

class Ship : public Actor
{
private:
	MoveComponent* mMoveComponent;
	SpriteComponent* mSpriteComponent;
	const float ANGULAR_SPEED = 8.0f;
	const float FORWARD_SPEED = 300.0f;
	const float COOLDOWN_FACTOR = 1.0f;
	bool mIsMoving;
	float mCooldown = 0.0f;

public:
	Ship(Game* game);
	~Ship();
	MoveComponent* GetMoveComponent() const;
	SpriteComponent* GetSpriteComponent() const;
	bool CheckMovement() const { return mIsMoving; }
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
};

#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "Frog.h"

class Vehicle : public Actor
{
private:
	CollisionComponent* mCollisionComponent;
	Vector2 mMoveD;
	WrappingMove* mWrappingMove;
	Frog* mFrog;
	Vector2 mFrogPos;
	float mNormalSpeed;
	const float VEHICLE_SPEED = 50.0f;
	const float CAR_COLLIDER_SIZE = 32.0f;
	const float T_COLLIDER_SIZE_X = 64.0f;
	const float T_COLLIDER_SIZE_Y = 24.0f;
	const Vector2 TO_RIGHT = Vector2(1, 0);
	const Vector2 TO_LEFT = Vector2(-1, 0);

public:
	Vehicle(Game* game, std::string s, float x, float y, int row);
	~Vehicle();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	WrappingMove* GetWrappingMove() const { return mWrappingMove; }
	Frog* GetFrog() const { return mFrog; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	void SetWrappingMove(WrappingMove* w) { mWrappingMove = w; }
	void OnUpdate(float deltaTime) override;
	void SetFrog(Frog* frog) { mFrog = frog; }
};

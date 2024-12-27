#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "Frog.h"
class Frog;
class Log : public Actor
{
private:
	CollisionComponent* mCollisionComponent;
	Vector2 mMoveD;
	WrappingMove* mWrappingMove;
	Frog* mFrog;
	Vector2 mFrogPos;
	float mNormalSpeed;
	bool mIsRide = false;
	const Vector2 TO_RIGHT = Vector2(1, 0);
	const Vector2 TO_LEFT = Vector2(-1, 0);
	const float LOG_SPEED = 37.5f;
	const float LOG_COLLIDER_HEIGHT = 24.0f;
	const float LOG_COLLIDER_X = 96.0f;
	const float LOG_COLLIDER_Y = 128.0f;
	const float LOG_COLLIDER_Z = 192.0f;

public:
	Log(Game* game, std::string s, float x, float y, int row);
	~Log();
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	WrappingMove* GetWrappingMove() const { return mWrappingMove; }
	Frog* GetFrog() const { return mFrog; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	void SetWrappingMove(WrappingMove* w) { mWrappingMove = w; }
	void SetFrog(Frog* frog) { mFrog = frog; }
	bool GetIsRide() const { return mIsRide; }
	void SetIsRide(bool i) { mIsRide = i; }
};

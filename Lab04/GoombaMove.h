#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
class Game;
class Actor;
class CollisionComponent;
class Goomba;
class GoombaMove : public MoveComponent
{
public:
	Vector2 GetDirection() const;
	void SetDirection(const Vector2& direction);
	GoombaMove(Actor* actor, CollisionComponent* colllisoncomponent, Goomba* goomba);
	~GoombaMove();
	void Update(float deltaTime) override;
	float GetDeltatime() const { return mDeltatime; }
	void SetDeltatime(float d) { mDeltatime = d; }
	float GetYSpeed() const { return mYSpeed; }
	void SetYSpeed(float y) { mYSpeed = y; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	Vector2 GetOffset() const { return mOffset; }
	void SetOffset(Vector2 v) { mOffset = v; }
	bool GetSpacePressed() const { return mSpacePressed; }
	void SetSpacePressed(bool v) { mSpacePressed = v; }
	bool GetInAir() const { return mInAir; }
	void SetInAir(bool v) { mInAir = v; }
	Goomba* GetGoomba() const { return mGoomba; }
	void SetGoomba(Goomba* v) { mGoomba = v; }
	float GetStompedTime() const { return mStompedTime; }
	void SetStompedTime(float v) { mStompedTime = v; }
	float const DESTROY_TIME = 0.25f;

private:
	Vector2 mDirection;
	CollisionComponent* mCollisionComponent = nullptr;
	const float WINDOW_HEIGHT = 448.0f;
	float mDeltatime;
	float const FORWARD_SPEED = -100.0f;
	float mYSpeed = 0.0f;
	float const GRAVITY_SPEED = 2000.0f;
	Vector2 mOffset;
	bool mSpacePressed = false;
	bool mInAir = false;
	Goomba* mGoomba = nullptr;
	float mStompedTime = 0.0f;
	float const HALF_JUMP = -350.0f;
	bool mIsUpdate = false;
};

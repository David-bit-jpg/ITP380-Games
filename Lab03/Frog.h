#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "WrappingMove.h"
#include "Log.h"
class CollisionComponent;
class SpriteComponent;
class Frog : public Actor
{
private:
	CollisionComponent* mCollisionComponent;
	Vector2 mOffset;
	const float WINDOW_WIDTH = 448.0f;
	const float WINDOW_HEIGHT = 512.0f;
	const float Y_LOWER_BOUND = 90.0f;
	const float Y_UPPER_BOUND = 255.0f;
	const float GOAL_Y = 80.0f;
	const float GOAL_X = 224.0f;
	const float SPEED_BALANCE = 12.0f;
	float mOnLog = false;
	Vector2 mInitialPos;
	bool TestInRange(Vector2 newPos) const;
	const float FROG_COLLIDER_SIZE = 25.0f;
	void CheckGoal();
	bool mMoved = false;
	const float SQUARE_LENGTH = 32.0f;
	void OnProcessInput(const Uint8* keyState) override;
	std::unordered_map<SDL_Scancode, bool> mLastFrame;

public:
	Frog(Game* game, std::string s, float x, float y, int row);
	~Frog();
	void OnUpdate(float deltaTime) override;
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	Vector2 GetOffset() const { return mOffset; }
	void SetOffset(Vector2 v) { mOffset = v; }
	bool GetMoved() const { return mMoved; }
	void SetMoved(bool v) { mMoved = v; }
	const std::unordered_map<SDL_Scancode, bool>& GetLastFrame() const { return mLastFrame; }
	void AddToLastFrame(SDL_Scancode key, bool value) { mLastFrame[key] = value; }
};

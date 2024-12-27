#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
class Game;
class Actor;
class CollisionComponent;

class PlayerMove : public MoveComponent
{
public:
	Vector2 GetDirection() const;
	void SetDirection(const Vector2& direction);
	PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent);
	~PlayerMove();
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
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

private:
	Vector2 mDirection;
	CollisionComponent* mCollisionComponent = nullptr;
	const float WINDOW_HEIGHT = 448.0f;
	const float WINDOW_WIDTH = 600.0f;
	const float FALL_FACTOR = 50.0f;
	float mDeltatime;
	float const FORWARD_SPEED = 300.0f;
	float mYSpeed = 0.0f;
	float const GRAVITY_SPEED = 2000.0f;
	float const WIN_X = 6368.0f;
	float const JUMP_SPEED = 700.0f;
	Vector2 mOffset;
	bool mSpacePressed = false;
	bool mInAir = false;
	std::unordered_map<SDL_Scancode, bool> mLastFrame;
	const std::unordered_map<SDL_Scancode, bool>& GetLastFrame() const { return mLastFrame; }
	void AddToLastFrame(SDL_Scancode key, bool value) { mLastFrame[key] = value; }
	void UpdatePlayerNormalAnimation(AnimatedSprite* mPlayerSpriteComponent) const;
};

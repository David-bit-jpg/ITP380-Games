#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include "Sword.h"
class Game;
class Actor;
class CollisionComponent;
class Sword;
class Player;
class PlayerMove : public MoveComponent
{
	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};

public:
	Vector2 GetDirection() const;
	void SetDirection(const Vector2& direction);
	PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent);
	PlayerMove::Direction GetFaceDirection() const;
	void SetFaceDirection(const PlayerMove::Direction& facedirection);
	~PlayerMove();
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	float GetDeltatime() const { return mDeltatime; }
	void SetDeltatime(float d) { mDeltatime = d; }
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	Vector2 GetOffset() const { return mOffset; }
	void SetOffset(Vector2 v) { mOffset = v; }
	bool GetSpacePressed() const { return mSpacePressed; }
	void SetSpacePressed(bool v) { mSpacePressed = v; }
	Sword* GetSword() const { return mSword; }

private:
	Vector2 mDirection;
	Direction mFaceDirection = Direction::None;
	const float PLAYER_SPEED = 150.0f;
	CollisionComponent* mCollisionComponent = nullptr;
	const float WINDOW_HEIGHT = 448.0f;
	const float WINDOW_WIDTH = 600.0f;
	const float FALL_FACTOR = 50.0f;
	float mDeltatime;
	float const FORWARD_SPEED = 300.0f;
	float const WIN_X = 6368.0f;
	const float CAMERA_OFFSETX = -256.0f;
	const float CAMERA_OFFSETY = -224.0f;
	const float SWORD_SIZE_ONE = 20.0f;
	const float SWORD_SIZE_TWO = 28.0f;
	const Vector2 UP_OFFSET = Vector2(0.0f, -40.0f);
	const Vector2 DOWN_OFFSET = Vector2(0.0f, 40.0f);
	const Vector2 RIGHT_OFFSET = Vector2(32.0f, 0.0f);
	const Vector2 LEFT_OFFSET = Vector2(-32.0f, 0.0f);
	Vector2 mOffset;
	bool mIsMoving = false;
	bool mSpacePressed = false;
	void UpdatePlayerNormalAnimation(AnimatedSprite* mPlayerSpriteComponent) const;
	void UpdateAnimation();
	bool mIsAttacking = false;
	float mAttackTimer = 0.0f;
	const float ATTACK_DURATION = 0.25f;
	Sword* mSword = nullptr;
	Player* mPlayer;
};

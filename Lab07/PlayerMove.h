#pragma once
#include "SDL2/SDL.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Player.h"
#include "MoveComponent.h"
#include "SideBlock.h"
#include "Block.h"
#include "AudioSystem.h"
class Block;
class Player;
class SideBlock;
class MoveComponent;
class Actor;
class Game;
class CollisionComponent;
class PlayerMove : public MoveComponent
{
public:
	PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent);
	~PlayerMove();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; };
	float GetMultipler() const { return mMultiplier; }
	int GetShield() const { return mShield; }

private:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void SpawnBlocks(float i);
	void Explode(Vector3 x, Block* bb, std::vector<Block*>& mBlocks);
	void BarrelRoll(float deltaTime);

	CollisionComponent* mCollisionComponent = nullptr;
	Vector3 mMovement = Vector3::Zero;
	float mSideBlockPos = 4000.0f;
	int mCntOne = -1;
	int mCntTwo = -1;
	int mCntBlockOne = -1;
	int mCntBlockTwo = -1;
	int mSideBlockRound = 0;
	int mBlockLevel = 5;
	float mSpacePressed = false;
	int mShield = 3;
	float mShieldTimer = 0.0f;
	float mMultiplier = 1.0f;
	float mMultiplierTimer = 0.0f;
	bool mQPressed = false;
	bool mIsRolling = false;
	float mRollTimer = 0.0f;
	float mReminderTimer = 0.0f;
	bool mAlertPlaying = false;
	SoundHandle mAlertSound;

	const Vector3 VELOCITY_AUTO_X = Vector3(400.0f, 0.0f, 0.0f);
	const Vector3 VELOCITY_W = Vector3(0.0f, 0.0f, 300.0f);
	const Vector3 VELOCITY_S = Vector3(0.0f, 0.0f, -300.0f);
	const Vector3 VELOCITY_A = Vector3(0.0f, -300.0f, 0.0f);
	const Vector3 VELOCITY_D = Vector3(0.0f, 300.0f, 0.0f);
	const Vector3 SBONE = Vector3(0.0f, 500.0f, 0.0f);
	const Vector3 SBTWO = Vector3(0.0f, -500.0f, 0.0f);
	const Vector3 SBTHREE = Vector3(0.0f, 0.0f, -500.0f);
	const Vector3 SBFOUR = Vector3(0.0f, 0.0f, 500.0f);
	const float MAX_Y = 180.0f;
	const float MAX_Z = 225.0f;
	const float HDIST = 300.0f;
	const float TARGETDIST = 20.0f;
	const float STEPSIZE = 500.0f;
	const float RANGE = 50.0f;
	const float INCREASE = 0.15f;
	const float ROLLTIME = 0.5f;
	const float ROLL_SPEED = 8.0f * Math::Pi;
};

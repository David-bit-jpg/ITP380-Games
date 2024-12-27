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
#include "AudioSystem.h"
#include "CollisionComponent.h"
#include "Crosshair.h"
#include "SegmentCast.h"
#include "Portal.h"
class Portal;
class SegmentCast;
class Crosshair;
class Player;
class MoveComponent;
class Actor;
class Game;
class CollisionComponent;
enum class MoveState
{
	OnGround,
	Jump,
	Falling
};
class PlayerMove : public MoveComponent
{
public:
	PlayerMove(Actor* actor);
	~PlayerMove();

	float GetMultipler() const { return mMultiplier; }
	Vector3& GetVelocity() { return mVelocity; }
	Vector3& GetAcceleration() { return mAcceleration; }
	void SetVelocity(Vector3 v) { mVelocity = v; }
	void SetAcceleration(Vector3 v) { mAcceleration = v; }
	void CollectGun(Actor* gun);
	Crosshair* GetCrossHair() const { return mCrosshair; }
	void ChangeState(MoveState ms) { mCurrentState = ms; }
	void Taunt();

private:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
					  const Vector2& relativeMouse) override;

	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);

	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }
	void FixXYVelocity();
	void CreatePortal(bool isBlue);
	bool UpdatePortalTeleport(float deltaTime);
	void CalculateTeleport(Portal* entryPort, Portal* exitPort);
	SoundHandle mDeathSound;

	CollSide FixCollision(CollisionComponent* self, CollisionComponent* collider);

	Vector3 mMovement = Vector3::Zero;
	float mMultiplier = 1.0f;
	MoveState mCurrentState = MoveState::OnGround;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass = 1.0f;
	Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	bool mSpacePressed = false;
	Crosshair* mCrosshair = nullptr;
	bool mLeftMousePressed = false;
	bool mRightMousePressed = false;
	bool mRPressed = false;
	float mTimeSinceLastTeleport = 0.0f;
	bool mFallingPortal = false;
	SoundHandle mFootstep;
	std::vector<std::string> mSounds = {"Glados-PlayerDead1.ogg", "Glados-PlayerDead2.ogg",
										"Glados-PlayerDead3.ogg", "Glados-PlayerDead4.ogg"};
	std::vector<std::string> mSubtitles = {
		"Congratulations! The test is now over.",
		"Thank you for participating in this Aperture Science computer-aided enrichment activity.",
		"Goodbye.", "You're not a good person. You know that, right?"};

	float const FORCE = 700.0f;
	float const MAX_SPEED = 400.0f;
	float const LINE_LEN = 1000.0f;
	float const MIN_Z = -1000.0f;
	float const STOPCOEE = 0.9f;
	float const CCX = 110.0f;
	float const CCY = 125.0f;
	float const CCZ = 10.0f;
	float const VTHRESHOLD = 350.0f;
	float const UPVALUE = 50.0f;
	float const RELOADZ = -750.0f;
	float const TELEPORT_INT = 0.2f;
	float const WALKSOUND_THR = 50.0f;
};

#pragma once
#include "SDL2/SDL.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Player.h"
#include "VehicleMove.h"
#include "AudioSystem.h"
class Player;
class VehicleMove;
class Actor;
class Game;
class CollisionComponent;
class PlayerMove : public VehicleMove
{
public:
	PlayerMove(Actor* actor);
	~PlayerMove();

	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; };
	float GetMultipler() const { return mMultiplier; }
	void SetCanMove(bool b) { mCanMove = b; }

private:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

	CollisionComponent* mCollisionComponent = nullptr;
	Vector3 mMovement = Vector3::Zero;
	float mMultiplier = 1.0f;
	void OnLapChange(int newLap) override;
	bool mCanMove = true;

	int const INIT_R = 39;
	int const INIT_C = 58;

	float const MIN_ACC = 1000.0f;
	float const MAX_ACC = 2500.0f;
	float const RAMP_TIME = 2.0f;
	float const ANGULAR_ACC = 5.0f * Math::Pi;
	float const LINEDRAG_COFF_PRESSED = 0.9f;
	float const LINEDRAG_COFF_NOTPRESSED = 0.975f;
	float const ANGULAR_COFF = 0.9f;
	const float FALLSPEED = 10.0f;
	const float TARGETZ = -100.0f;
};

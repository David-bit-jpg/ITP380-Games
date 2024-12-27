#pragma once
#include "SpriteComponent.h"
#include "Math.h"
#include "Soldier.h"
#include <vector>
#include <string>
class Soldier;
class PathNode;

class SoldierAI : public SpriteComponent
{
public:
	SoldierAI(class Actor* owner);

	// Setup this soldier for the correct patrol start/end
	// and start the initial movement
	void Setup(PathNode* start, PathNode* end);

	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer) override;

	Vector2 GetDirection() const { return mDirection; }

	void SetDirection(Vector2 s) { mDirection = s; }

	Soldier* GetSoldier() const { return mSoldier; }

	void SetSoldier(Soldier* s) { mSoldier = s; }

	void SetStunned(bool s) { mIsStunned = s; }
	// TODO: Add any public functions as needed
private:
	// The start path node for the patrol path
	PathNode* mPatrolStart = nullptr;
	// The end path node for the patrol path
	PathNode* mPatrolEnd = nullptr;

	// The previous node we were at for the current move
	PathNode* mPrev = nullptr;
	// The next node we're moving to for the current move
	PathNode* mNext = nullptr;
	// The rest of the path after next to target
	std::vector<PathNode*> mPath;

	// How many pixels/s the soldier movies
	const float SOLDIER_SPEED = 75.0f;
	// How long the soldier gets stunned when hit
	const float STUN_DURATION = 1.0f;

	Vector2 mDirection = Vector2::Zero;
	// TODO: Add any private data/functions as needed
	const float THRESHOLD = 1.0f;
	Soldier* mSoldier = nullptr;
	bool mIsStunned = false;
	float mStunnedTimer = 0.0f;

	const void UpdateAnimation() const;
};

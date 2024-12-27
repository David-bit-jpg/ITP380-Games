#pragma once
#include "Component.h"
#include "Math.h"
#include "PathNode.h"
#include <vector>
class PathNode;
class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};

	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;

	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);

	// Get the current state
	State GetState() const { return mState; }

	// Called when the ghost should switch to the "Frightened" state
	void Frighten();

	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current goal, prev, and next
	void DebugDrawPath(struct SDL_Renderer* render);

private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying to get to
	class PathNode* mNextNode = nullptr;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;
	Vector2 mDirection;
	Vector2 mPrevDirection;
	Vector2 GetDirection() const { return mDirection; }
	Vector2 CalculateDirection();
	void SetDirection(Vector2 d) { mDirection = d; }
	Ghost* GetGhost() const { return mGhost; }
	void SetState(State s) { mState = s; }
	void SetPrevNode(PathNode* s) { mPrevNode = s; }
	void SetNextNode(PathNode* s) { mNextNode = s; }
	void SetTargetNode(PathNode* s) { mTargetNode = s; }
	PathNode* GetPrevNode() const { return mPrevNode; }
	PathNode* GetNextNode() const { return mNextNode; }
	PathNode* GetClosestNode(PathNode* node);
	PathNode* GetClosestNodeGhost(PathNode* node);
	PathNode* GetClosestNodeAny(PathNode* node);
	PathNode* GetRandomNode(PathNode* node);
	PathNode* GetClosestNodeDefault(PathNode* node);
	PathNode* GetClosestNodeByPos(Vector2 node);
	float const SCATTER_CHASE_SPEED = 90.0f;
	float const FRIGHTENED_SPEED = 65.0f;
	float const DEAD_SPEED = 125.0f;
	float mInStateTime = 0.0f;
	float GetInStateTime() const { return mInStateTime; }
	void SetInStateTime(float f) { mInStateTime = f; }
	void IsChangeState();
	float const CHANGE_STATE_FS = 7.0f;
	float const CHANGE_STATE_SC = 5.0f;
	float const CHANGE_STATE_CS = 20.0f;
	float const ANIMATION_CHANGE = 5.0f;
	float const POINT_IN_FRONT_CHASE = 80.0f;
	float const CLYDE_THRESHOLD = 150.0f;
	static const int GHOST_COUNT = 4;
	PathNode* GetChaseNode();
};

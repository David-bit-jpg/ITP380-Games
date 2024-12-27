#pragma once
#include "Actor.h"
#include <string>
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
class Ghost : public Actor
{
public:
	// Which Ghost is this?
	enum Type
	{
		Blinky,
		Pinky,
		Inky,
		Clyde
	};

	Ghost(class Game* game, Type type);

	// Get/set the spawn node and scatter target nodes
	void SetSpawnNode(class PathNode* node) { mSpawnNode = node; }
	void SetScatterNode(class PathNode* node) { mScatterNode = node; }
	class PathNode* GetSpawnNode() const { return mSpawnNode; }
	class PathNode* GetScatterNode() const { return mScatterNode; }

	// Start is called when the Ghost should begin its initial behavior
	void Start();

	// Get the type of ghost this is
	Type GetType() const { return mType; }

	// A ghost becomes "frightened" when Pac-Man picks up the power pellet
	void Frighten();
	// Returns true if the ghost is currently frightened
	bool IsFrightened() const;

	// When Pac-Man eats a ghost that's frightened, it "dies"
	void Die();
	// Is this ghost currently dead?
	bool IsDead() const;

	// Helper function that sets correct color for ghost path drawing
	void DebugDrawPath(struct SDL_Renderer* render);
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	AnimatedSprite* GetAnimatedSprite() const { return mAnimatedSprite; }
	void SetAnimatedSprite(AnimatedSprite* c) { mAnimatedSprite = c; }

private:
	void SetupMoveAnim(const std::string& name);
	Type mType;

	class PathNode* mSpawnNode;
	class PathNode* mScatterNode;
	class GhostAI* mAI;
	AnimatedSprite* mAnimatedSprite = nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
};

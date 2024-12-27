#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include <vector>
class CollisionComponent;
class PathNode : public Actor
{
public:
	enum Type
	{
		Default,
		Ghost,
		Tunnel
	};
	PathNode(class Game* game, Type type);

	Type GetType() const { return mType; }

	// An id for the node, to aid in debugging.
	int mNumber = 0;

	std::vector<PathNode*> mAdjacent;
	CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	void SetCollisionComponent(CollisionComponent* c) { mCollisionComponent = c; }
	std::vector<PathNode*> GetAdjacentNodes() const { return mAdjacent; };

private:
	Type mType;
	CollisionComponent* mCollisionComponent = nullptr;
};

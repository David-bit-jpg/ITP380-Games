#pragma once
#include "Actor.h"
#include <vector>
#include <unordered_map>
class PathNode;
// This is a helper actor which doesn't have any components
// but tracks all of the path nodes in the game and where
// you'll calculate a path using A*
struct NodeInfo
{
	// The parent is the node at "before" current node
	PathNode* mParent = nullptr;
	// f(x) = g(x) + h(x)
	float mF = 0.0f;
	// g(x) true cost from start to this node
	float mG = 0.0f;
	// h(x) heuristic estimate from this node to goal
	float mH = 0.0f;
	// Whether or not this node is "closed"
	bool mIsClosed = false;
};
class PathFinder : public Actor
{

public:
	PathFinder(class Game* game);

	// Calculate an A* path from start to end, storing the path in outPath
	// Returns false if it fails to find a path, true otherwise
	bool CalculatePath(class PathNode* start, class PathNode* end,
					   std::vector<class PathNode*>& outPath);

	// Returns a pointer to the path node at the specified row/col
	// (Or nullptr if there is no node there)
	class PathNode* GetPathNode(size_t row, size_t col);

	// Use this to change whether the PathNode at row/col is blocked
	void SetIsBlocked(size_t row, size_t col, bool isBlocked);

	// Debug function that can draw all the edges in the graph
	// (This isn't necessary to use generally)
	void DebugDrawPaths(struct SDL_Renderer* renderer);

private:
	// 2D dynamic array of all the path nodes in the world
	std::vector<std::vector<class PathNode*>> mPathNodeGrid;

	// Helper function to generate the pathnodes from the map data
	void InitializePathNodes();

	PathNode* FindSmallestF(std::vector<PathNode*> openSet,
							std::unordered_map<PathNode*, NodeInfo> nodeInfos);

	void BuildPath(std::vector<class PathNode*>& outPath, PathNode* end,
				   std::unordered_map<PathNode*, NodeInfo> nodeInfos);

	void Initialize(std::unordered_map<PathNode*, NodeInfo>& nodeInfos, PathNode* end);
};

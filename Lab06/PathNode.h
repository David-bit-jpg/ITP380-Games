#pragma once
#include "Actor.h"
#include <vector>

class PathNode : public Actor
{
public:
	PathNode(class Game* game, size_t row, size_t col);

	bool IsBlocked() const { return mIsBlocked; }

	std::vector<PathNode*> mAdjacent;

protected:
	size_t mRow = 0;
	size_t mCol = 0;

private:
	friend class PathFinder;
	void SetIsBlocked(bool isBlocked);
	bool mIsBlocked = false;
};

#pragma once
#include "Actor.h"

class PacMan : public Actor
{
public:
	PacMan(class Game* game);

	void SetSpawnNode(class PathNode* node) { mSpawnNode = node; }

	class PathNode* GetSpawnNode() const { return mSpawnNode; }

	class PathNode* GetPrevNode() const;

	Vector2 GetPointInFrontOf(float dist) const;

	void DoGameIntro();

private:
	class PathNode* mSpawnNode;
};

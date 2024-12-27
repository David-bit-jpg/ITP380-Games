#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "EnemyMove.h"
#include <vector>
class EnemyMove;
class MeshComponent;
class PlayerMove;
class Game;
class Enemy : public Actor
{
private:
	MeshComponent* mMeshComponent = nullptr;
	EnemyMove* mEnemyMove = nullptr;
	std::vector<Vector3> mVector;
	int mRow = 0;
	int mCol = 0;

	float const SCALE = 0.75f;

	void ReadCSV(std::string filename);

public:
	Enemy(Game* game, std::string filename);
	~Enemy();
	std::vector<Vector3> GetNode() const { return mVector; }
	EnemyMove* GetEnemyMove() const { return mEnemyMove; }
};

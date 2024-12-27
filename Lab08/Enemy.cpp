#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "Enemy.h"
#include "Renderer.h"
#include "EnemyMove.h"
#include <vector>
#include "CSVHelper.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include "HeightMap.h"
class HeightMap;
class EnemyMove;
class Game;
class Renderer;
Enemy::Enemy(Game* game, std::string filename)
: Actor(game)
{
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mc->SetTextureIndex(6);
	mMeshComponent = mc;
	EnemyMove* em = new EnemyMove(this);
	mEnemyMove = em;
	ReadCSV(filename);
}

Enemy::~Enemy()
{
}

void Enemy::ReadCSV(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	std::vector<Vector3> mTemp;
	std::getline(file, line);
	while (!file.eof())
	{
		mRow++;
		std::getline(file, line);
		if (!line.empty())
		{
			std::vector<std::string> mLineInfo = CSVHelper::Split(line);
			if (!mLineInfo.empty())
			{
				mCol++;
				int mX = std::stoi(mLineInfo[1]);
				int mY = std::stoi(mLineInfo[2]);
				HeightMap* hm = GetGame()->GetHeightMap();
				Vector3 worldPos = hm->CellToWorld(mX, mY);
				mTemp.push_back(worldPos);
			}
		}
	}
	mRow -= 1;
	mCol = mCol / mRow;
	mVector = mTemp;
	SetPosition(mVector[0]);
}
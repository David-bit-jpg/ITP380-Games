#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Math.h"

class HeightMap
{
public:
	HeightMap(std::string filename);
	~HeightMap();

	Vector3 CellToWorld(int row, int col);
	Vector2 WorldToCell(const Vector2& pos) const;
	bool IsOnTrack(const Vector2& pos);
	float GetHeight(const Vector2& pos);

	void SetInts(std::vector<int> i) { mInts = i; }
	std::vector<int> GetInts() const { return mInts; }

private:
	std::vector<int> mInts;
	int mRow = 0;
	int mCol = 0;

	float const CELL_SIZE = 40.55f;
	float const GRID_TOP = 1280.0f;
	float const GRID_LEFT = -1641.0f;

	bool IsCellOnTrack(int row, int col);
	float GetHeightFromCell(int row, int col);
};

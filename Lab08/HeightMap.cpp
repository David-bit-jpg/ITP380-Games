#include "HeightMap.h"
#include "CSVHelper.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
HeightMap::HeightMap(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	std::vector<int> mTemp;
	while (!file.eof())
	{
		mRow++;
		std::getline(file, line);
		if (!line.empty())
		{
			std::vector<std::string> mLineInfo = CSVHelper::Split(line);
			if (!mLineInfo.empty())
			{
				for (std::string s : mLineInfo)
				{
					mCol++;
					mTemp.push_back(std::stoi(s));
				}
			}
		}
	}
	mRow -= 1;
	mCol = mCol / mRow;
	mInts = mTemp;
}

HeightMap::~HeightMap()
{
}

bool HeightMap::IsCellOnTrack(int row, int col)
{
	if (row < 0 || col < 0 || col > mCol || row > mRow)
	{
		return false;
	}
	int index = row * mCol + col;
	return mInts[index] != -1;
}
float HeightMap::GetHeightFromCell(int row, int col)
{
	int index = row * mCol + col;
	return -40.0f + mInts[index] * 5.0f;
}
Vector3 HeightMap::CellToWorld(int row, int col)
{
	if (!IsCellOnTrack(row, col))
	{
		return Vector3::Zero;
	}
	float x = GRID_TOP - CELL_SIZE * row;

	float y = GRID_LEFT + CELL_SIZE * col;

	float z = GetHeightFromCell(row, col);
	return Vector3(x, y, z);
}
Vector2 HeightMap::WorldToCell(const Vector2& pos) const
{
	int row = static_cast<int>((Math::Abs(pos.x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE);
	int col = static_cast<int>((Math::Abs(pos.y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE);
	return Vector2(row, col);
}
bool HeightMap::IsOnTrack(const Vector2& pos)
{
	Vector2 mTemp = WorldToCell(pos);
	return IsCellOnTrack(static_cast<int>(mTemp.x), static_cast<int>(mTemp.y));
}
float HeightMap::GetHeight(const Vector2& pos)
{
	if (!IsOnTrack(pos))
	{
		return -1000;
	}
	Vector2 mTemp = WorldToCell(pos);
	return GetHeightFromCell(static_cast<int>(mTemp.x), static_cast<int>(mTemp.y));
}
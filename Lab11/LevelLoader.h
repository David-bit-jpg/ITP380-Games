#pragma once
#include <string>

class LevelLoader
{
public:
	static bool Load(class Game* game, const std::string& fileName);
};

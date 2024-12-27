#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CSVHelper.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

TiledBGComponent::~TiledBGComponent()
{
}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		std::vector<SDL_Rect> mTemp;
		int mTexRow = GetTexHeight() / mTileHeight;
		int mTexCol = GetTexWidth() / mTileWidth;
		for (int row = 0; row < mTexRow; row++) //find the source texture, and divide it
		{
			for (int col = 0; col < mTexCol; col++)
			{
				SDL_Rect sourceRect;
				sourceRect.w = mTileWidth;
				sourceRect.h = mTileHeight;
				sourceRect.x = col * mTileWidth;
				sourceRect.y = row * mTileHeight;
				mTemp.push_back(sourceRect);
			}
		}
		for (int row = 0; row < mRow;
			 row++) //find the destination of rectangles by iterating the saved numbers
		{
			for (int col = 0; col < mCol; col++)
			{
				int tileNum = mIntegers[row][col];
				if (tileNum == -1)
					continue;
				SDL_Rect destRect;
				destRect.w = mTileWidth;
				destRect.h = mTileHeight;
				destRect.x =
					static_cast<int>(col * mTileWidth - GetGame()->GetCameraPos().x); //col pos
				destRect.y =
					static_cast<int>(row * mTileHeight - GetGame()->GetCameraPos().y); //row pos
				SDL_RenderCopyEx(renderer, mTexture, &mTemp.at(tileNum), &destRect, 0.0, nullptr,
								 SDL_FLIP_NONE);
			}
		}
	}
}
void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
	std::ifstream file(fileName);
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;
	if (!file.is_open())
	{
		SDL_Log("Failed to load level: %s", fileName.c_str());
	}
	std::string line;
	std::vector<int> mTemp;
	while (!file.eof())
	{
		std::getline(file, line);
		if (!line.empty())
		{
			for (std::string s : CSVHelper::Split(line)) //split the line read
			{
				mCol++;
				mTileNum++;
				mTemp.push_back(std::stoi(s));
			}
			mIntegers.push_back(mTemp); //find the number and push back
			mTemp.clear();
			mRow++;
		}
	}
	mCol /= mRow;
}
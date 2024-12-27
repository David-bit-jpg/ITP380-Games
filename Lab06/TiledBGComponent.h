#include "SpriteComponent.h"
#include <vector>
#include <string>
class Game;
class Actor;
class TiledBGComponent : public SpriteComponent
{
public:
	TiledBGComponent(class Actor* owner, int drawOrder = 50);
	~TiledBGComponent();

	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
	void Draw(SDL_Renderer* renderer) override;

protected:
	std::vector<std::vector<int>> mIntegers;
	std::vector<std::vector<int>> GetIntegers() const { return mIntegers; }

	int mRow = 0;
	int mCol = 0;
	int mTileNum = 0;
	int mTileWidth = 0;
	int mTileHeight = 0;
};

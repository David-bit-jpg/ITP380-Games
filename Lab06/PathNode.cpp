#include "PathNode.h"
#include "SpriteComponent.h"
#include "Game.h"
#include <string>

PathNode::PathNode(class Game* game, size_t row, size_t col)
: Actor(game)
, mRow(row)
, mCol(col)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Node.png"));
	sc->SetIsVisible(false); // Comment this out to see path nodes
}

void PathNode::SetIsBlocked(bool isBlocked)
{
	mIsBlocked = isBlocked;
	SpriteComponent* sc = GetComponent<SpriteComponent>();
	if (!mIsBlocked)
	{
		sc->SetTexture(mGame->GetTexture("Assets/Node.png"));
	}
	else
	{
		sc->SetTexture(mGame->GetTexture("Assets/NodeBlocked.png"));
	}
}

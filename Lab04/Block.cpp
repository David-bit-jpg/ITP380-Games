#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SDL2/SDL.h"
#include "Block.h"
class CollisionComponent;
class SpriteComponent;
class Game;

Block::Block(Game* game, std::string s, float x, float y, int row)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mBCollision = new CollisionComponent(this);
	mBCollision->SetSize(BLOCK_SIZE, BLOCK_SIZE);
	SpriteComponent* sprite = new SpriteComponent(this);
	SetCollisionComponent(mBCollision);
	sprite->SetTexture(GetGame()->GetTexture(s));
	mSpriteComponent = sprite;
}

Block::~Block()
{
	GetGame()->RemoveBlock(this);
}
void Block::OnUpdate(float deltaTime)
{
}

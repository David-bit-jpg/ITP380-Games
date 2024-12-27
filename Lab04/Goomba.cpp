#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SDL2/SDL.h"
#include "Goomba.h"
#include "GoombaMove.h"
#include "AnimatedSprite.h"
class AnimatedSprite;
class CollisionComponent;
class SpriteComponent;
class Game;
class GoombaMove;
Goomba::Goomba(Game* game, float x, float y)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mBCollision = new CollisionComponent(this);
	mBCollision->SetSize(GOOMBA_SIZE, GOOMBA_SIZE);
	GoombaMove* gm = new GoombaMove(this, mBCollision, this);
	mGoombaMove = gm;
	AnimatedSprite* sprite = new AnimatedSprite(this, 150);
	std::vector<SDL_Texture*> walkAnim{GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
									   GetGame()->GetTexture("Assets/Goomba/Walk1.png")};
	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Goomba/Dead.png")};
	sprite->AddAnimation("walk", walkAnim);
	sprite->AddAnimation("dead", deadAnim);
	SetCollisionComponent(mBCollision);
	// sprite->SetTexture(GetGame()->GetTexture("Assets/Goomba/Walk0.png"));
	SetSpriteComponent(sprite);
}

Goomba::~Goomba()
{
	GetGame()->RemoveSprite(GetSpriteComponent());
	GetGame()->RemoveGoomba(this);
}
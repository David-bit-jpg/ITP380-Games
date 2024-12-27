#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SDL2/SDL.h"
#include "Player.h"
#include "PlayerMove.h"
#include "AnimatedSprite.h"
class AnimatedSprite;
class CollisionComponent;
class SpriteComponent;
class Game;
class PlayerMove;

Player::Player(Game* game, float x, float y)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mBCollision = new CollisionComponent(this);
	mBCollision->SetSize(PLAYER_COLLIDER_SIZE, PLAYER_COLLIDER_SIZE);
	PlayerMove* mPlayerMove = new PlayerMove(this, mBCollision);
	GetGame()->SetPlayerMovement(mPlayerMove);
	AnimatedSprite* sprite = new AnimatedSprite(this);
	sprite->LoadAnimations("Assets/Link");
	sprite->SetAnimation("StandDown");
	GetGame()->AddSprite(sprite);
	SetCollisionComponent(mBCollision);
	SetSpriteComponent(sprite);
}

Player::~Player()
{
}
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
	PlayerMove* mPlayerMove = new PlayerMove(this, mBCollision);
	GetGame()->SetPlayerMovement(mPlayerMove);
	mBCollision->SetSize(PLAYER_SIZE, PLAYER_SIZE);
	AnimatedSprite* sprite = new AnimatedSprite(this, 200);
	SetCollisionComponent(mBCollision);
	std::vector<SDL_Texture*> idleAnim{GetGame()->GetTexture("Assets/Mario/Idle.png")};
	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Mario/Dead.png")};
	std::vector<SDL_Texture*> jumpLeftAnim{GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};
	std::vector<SDL_Texture*> jumpRight{GetGame()->GetTexture("Assets/Mario/JumpRight.png")};
	std::vector<SDL_Texture*> runLeft{GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
									  GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
									  GetGame()->GetTexture("Assets/Mario/RunLeft2.png")};
	std::vector<SDL_Texture*> runRight{GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
									   GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
									   GetGame()->GetTexture("Assets/Mario/RunRight2.png")};

	sprite->AddAnimation("Idle", idleAnim);
	sprite->AddAnimation("Dead", deadAnim);
	sprite->AddAnimation("JumpLeft", jumpLeftAnim);
	sprite->AddAnimation("JumpRight", jumpRight);
	sprite->AddAnimation("RunLeft", runLeft);
	sprite->AddAnimation("RunRight", runRight);
	SetSpriteComponent(sprite);
}

Player::~Player()
{
}
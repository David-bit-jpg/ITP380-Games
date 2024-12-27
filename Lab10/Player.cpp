#include "Actor.h"
#include "SDL2/SDL.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
Player::Player(Game* game)
: Actor(game)
{
	PlayerMove* pm = new PlayerMove(this);
	mPlayerMove = pm;
	CameraComponent* cm = new CameraComponent(this);
	mCameraComponent = cm;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CC_X, CC_Y, CC_Z);
	mCollisionComponent = cc;
	game->SetPlayer(this);
}

Player::~Player()
{
}
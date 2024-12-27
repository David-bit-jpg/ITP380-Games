#include "Actor.h"
#include "SDL2/SDL.h"
#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "HUD.h"
class HUD;
class PlayerMove;
class CollisionComponent;
class MeshComponent;
class Game;
class Renderer;

Player::Player(Game* game)
: Actor(game)
{
	SetScale(2.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arwing.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CCX, CCY, CCZ);
	mCollisionComponent = cc;
	PlayerMove* pm = new PlayerMove(this, mCollisionComponent);
	mPlayerMove = pm;
	HUD* hud = new HUD(this);
	mHUD = hud;
}

Player::~Player()
{
}
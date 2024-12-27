#include "Actor.h"
#include "SDL2/SDL.h"
#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "PlayerUI.h"
class PlayerUI;
class CameraComponent;
class PlayerMove;
class MeshComponent;
class Game;
class Renderer;
class PlayerUI;
Player::Player(Game* game)
: Actor(game)
{
	SetScale(0.75f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mMeshComponent = mc;
	PlayerMove* pm = new PlayerMove(this);
	mPlayerMove = pm;
	CameraComponent* cm = new CameraComponent(this);
	mCameraComponent = cm;
	mCameraComponent->SnapToIdeal();
	PlayerUI* pui = new PlayerUI(this);
	mPlayerUI = pui;
}

Player::~Player()
{
}
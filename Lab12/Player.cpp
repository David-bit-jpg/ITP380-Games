#include "Actor.h"
#include "SDL2/SDL.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "HUD.h"
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
	HUD* hud = new HUD(this);
	mHUD = hud;
	HealthComponent* hc = new HealthComponent(this);
	hc->SetOnDeathCallback([this]() {
		mPlayerMove->Taunt();
	});
	hc->SetOnDamageCallback([this](const Vector3& location) {
		Vector3 temp = location - GetPosition();
		temp.z = 0.0f;
		temp.Normalize();
		Vector3 forward = GetForward();
		forward.z = 0.0f;
		forward.Normalize();
		float angle = Math::Acos(Vector3::Dot(temp, forward));
		Vector3 cross = Vector3::Cross(temp, forward);
		float sign = cross.z >= 0 ? 1.0f : -1.0f;
		angle *= sign;
		mHUD->PlayerTakeDamage(angle);
	});
	mHealthComponent = hc;
}

Player::~Player()
{
}
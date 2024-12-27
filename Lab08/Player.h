#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "PlayerUI.h"
class PlayerUI;
class CameraComponent;
class PlayerMove;
class MeshComponent;
class Game;
class Player : public Actor
{
private:
	MeshComponent* mMeshComponent = nullptr;
	PlayerMove* mPlayerMove = nullptr;
	CameraComponent* mCameraComponent = nullptr;
	PlayerUI* mPlayerUI = nullptr;

public:
	Player(Game* game);
	~Player();

	PlayerMove* GetPlayerMove() const { return mPlayerMove; }
	PlayerUI* GetPlayerUI() const { return mPlayerUI; }
};

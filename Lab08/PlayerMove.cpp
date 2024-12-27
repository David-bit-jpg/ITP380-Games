#include "PlayerMove.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Player.h"
#include "VehicleMove.h"
#include "Renderer.h"
#include "Random.h"
class Player;
class Renderer;
class VehicleMove;
class Actor;
class Game;
class CollisionComponent;
PlayerMove::PlayerMove(Actor* actor)
: VehicleMove(actor)
{
	mOwner->SetPosition(GetGame()->GetHeightMap()->CellToWorld(INIT_R, INIT_C));
	SetMinAcc(MIN_ACC);
	SetMaxAcc(MAX_ACC);
	SetRampTime(RAMP_TIME);
	SetAngularAcc(ANGULAR_ACC);
	SetLinDragPressed(LINEDRAG_COFF_PRESSED);
	SetLinDragNotPressed(LINEDRAG_COFF_NOTPRESSED);
	SetAngularCoff(ANGULAR_COFF);
	SetFallSpeed(FALLSPEED);
	SetTargetZ(TARGETZ);
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Update(float deltaTime)
{
	VehicleMove::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (mCanMove)
	{
		SetPedalPressed(keyState[SDL_SCANCODE_W]);
		if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
		{
			SetTurnDirection(TurnDirection::Left);
		}
		else if (keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A])
		{
			SetTurnDirection(TurnDirection::Right);
		}
		else
		{
			SetTurnDirection(TurnDirection::None);
		}
	}
}
void PlayerMove::OnLapChange(int newLap)
{
	if (newLap == 2)
	{
		GetGame()->GetAudio()->StopSound(GetGame()->GetSoundHandle(), 250);
		GetGame()->GetAudio()->PlaySound("FinalLap.wav");
		SoundHandle sh = GetGame()->GetAudio()->PlaySound("MusicFast.ogg", true, 4000);
		GetGame()->SetSoundHandle(sh);
	}
	if (GetGame()->GetPlayer()->GetPlayerUI())
	{
		GetGame()->GetPlayer()->GetPlayerUI()->OnLapChange(newLap);
	}
}
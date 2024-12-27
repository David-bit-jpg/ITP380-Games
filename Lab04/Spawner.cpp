#include <string>
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SDL2/SDL.h"
#include "Spawner.h"
#include "Player.h"
class CollisionComponent;
class SpriteComponent;
class Game;
class Player;
class Goomba;

Spawner::Spawner(Game* game, float x, float y)
: Actor(game)
{
	SetPosition(Vector2(x, y));
}

Spawner::~Spawner()
{
	GetGame()->RemoveSpawner(this);
}
void Spawner::OnUpdate(float deltaTime)
{
	if (GetGame()->GetPlayer() != nullptr)
	{
		if (fabs(GetGame()->GetPlayer()->GetPosition().x - GetPosition().x) <= DISTANCE_SPAWN)
		{

			Goomba* mGoomba = new Goomba(GetGame(), GetPosition().x, GetPosition().y);
			GetGame()->AddGoomba(mGoomba);
			GetGame()->RemoveSpawner(this);
			SetState(ActorState::Destroy);
		}
	}
}

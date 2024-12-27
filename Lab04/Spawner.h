#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Player.h"
#include "CollisionComponent.h"
class CollisionComponent;
class SpriteComponent;
class Player;
class Goomba;
class Spawner : public Actor
{
private:
	void OnUpdate(float deltaTime) override;
	float const DISTANCE_SPAWN = 600.0f;

public:
	Spawner(Game* game, float x, float y);
	~Spawner();
};

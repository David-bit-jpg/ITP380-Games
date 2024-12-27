#pragma once
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"

class MoveComponent;
class SpriteComponent;

class Asteroid : public Actor
{
private:
	SpriteComponent* mSpriteComponent;
	MoveComponent* mMoveComponent;
	const float MOVESPEED = 150.0f;
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;

public:
	Asteroid(Game* game);
	~Asteroid();
	MoveComponent* GetMoveComponent() const;
	SpriteComponent* GetSpriteComponent() const;
	void OnUpdate(float deltaTime) override;
};

#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include "Game.h"
#include <string>
#include "AnimatedSprite.h"
class AnimatedSprite;
class Effect : public Actor
{
public:
	Effect(Game* game, Vector2 pos, std::string animiname, std::string soundname);
	~Effect();

	void SetLifeTime(float f) { mLifeTime = f; }

private:
	float mLifeTime = 0.0f;
	AnimatedSprite* mAnimatedSprite = nullptr;

	void OnUpdate(float deltaTime) override;
};

#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include "MoveComponent.h"
#include "Math.h"
#include <algorithm>
class Game;
class Actor;
class WrappingMove : public MoveComponent
{
public:
	Vector2 GetDirection() const;
	void SetDirection(const Vector2& direction);
	WrappingMove(Actor* actor);
	~WrappingMove();
	void Update(float deltaTime) override;
	float GetDeltatime() const { return mDeltatime; }
	void SetDeltatime(float d) { mDeltatime = d; }

private:
	Vector2 mDirection;
	const float WINDOW_WIDTH = 448.0f;
	float mDeltatime;
	// const int WINDOW_HEIGHT = 512;
};

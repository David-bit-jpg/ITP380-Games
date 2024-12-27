#pragma once
#include "UIComponent.h"

enum class CrosshairState
{
	Default,
	BlueFill,
	OrangeFill,
	BothFill
};

class Crosshair : public UIComponent
{
public:
	Crosshair(class Actor* owner);

	void Draw(class Shader* shader) override;
	void SetState(CrosshairState newState) { mState = newState; }
	CrosshairState GetState() const { return mState; }

private:
	CrosshairState mState = CrosshairState::Default;
};

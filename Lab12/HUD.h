#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "UIComponent.h"
#include "MeshComponent.h"
class Renderer;
class MeshComponent;
class UIComponent;
class Game;
class HUD : public UIComponent
{
private:
	void Draw(class Shader* shader) override;

	class Font* mFont = nullptr;
	class Texture* mSubTitleTexture = nullptr;
	class Texture* mSubTitleShadowTexture = nullptr;

	float const TIME = 1.5f;
	float const POS_Y = -325.0f;
	Vector2 const OFFSET = Vector2(2.0f, -2.0f);
	class Texture* mTexture;
	float mIndicatorAngle = 0.0f;
	float mIndicatorTime = 0.0f;
	void Update(float deltaTime) override;

public:
	HUD(class Actor* owner);
	~HUD();

	void ShowSubtitle(std::string s);
	void PlayerTakeDamage(float angle);
};

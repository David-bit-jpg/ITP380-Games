#include "Actor.h"
#include "SDL2/SDL.h"
#include "HUD.h"
#include "Game.h"
#include "Texture.h"
#include "Font.h"
#include "Renderer.h"
HUD::HUD(Actor* owner)
: UIComponent(owner)
{
	Font* font = new Font();
	font->Load("Assets/Inconsolata-Regular.ttf");
	mFont = font;
	mTexture = GetGame()->GetRenderer()->GetTexture("Assets/Textures/UI/DamageIndicator.png");
}

HUD::~HUD()
{
	if (mFont)
	{
		mFont->Unload();
		delete mFont;
	}
}
void HUD::Draw(class Shader* shader)
{
	if (mSubTitleTexture && mSubTitleShadowTexture)
	{
		Vector2 mScreenPos = Vector2(0.0f, POS_Y + mSubTitleTexture->GetHeight() / 2.0f);
		DrawTexture(shader, mSubTitleShadowTexture, mScreenPos + OFFSET);
		DrawTexture(shader, mSubTitleTexture, mScreenPos);
	}
	if (mIndicatorTime > 0.0f)
	{
		DrawTexture(shader, mTexture, Vector2::Zero, 1.0f, mIndicatorAngle);
	}
	if (GetGame()->GetPlayer()->GetComponent<HealthComponent>()->IsDead())
	{
		class Texture* texture =
			GetGame()->GetRenderer()->GetTexture("Assets/Textures/UI/DamageOverlay.png");
		DrawTexture(shader, texture);
	}
}
void HUD::ShowSubtitle(std::string s)
{
	std::string show;
	if (mSubTitleTexture)
	{
		mSubTitleTexture->Unload();
		if (mSubTitleShadowTexture)
			mSubTitleShadowTexture->Unload();
		delete mSubTitleTexture;
		delete mSubTitleShadowTexture;
		mSubTitleTexture = nullptr;
		mSubTitleShadowTexture = nullptr;
	}
	if (!s.empty())
	{
		show = "GLaDOS: " + s;
		mSubTitleTexture = mFont->RenderText(show, Color::LightGreen);
		mSubTitleShadowTexture = mFont->RenderText(show, Color::Black);
	}
}
void HUD::PlayerTakeDamage(float angle)
{
	mIndicatorAngle = angle;
	mIndicatorTime = TIME;
}
void HUD::Update(float deltaTime)
{
	mIndicatorTime -= deltaTime;
}

#include "Crosshair.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Crosshair::Crosshair(class Actor* owner)
: UIComponent(owner)
{
	// Cache the textures
	Renderer* r = owner->GetGame()->GetRenderer();
	r->GetTexture("Assets/Textures/UI/CrosshairDefault.png");
	r->GetTexture("Assets/Textures/UI/CrosshairBlueFill.png");
	r->GetTexture("Assets/Textures/UI/CrosshairOrangeFill.png");
	r->GetTexture("Assets/Textures/UI/CrosshairBothFill.png");
}

void Crosshair::Draw(class Shader* shader)
{
	Player* player = GetGame()->GetPlayer();

	if (player->HasGun())
	{
		std::string textureName = "Assets/Textures/UI/CrosshairDefault.png";
		switch (mState)
		{
		case CrosshairState::BlueFill:
			textureName = "Assets/Textures/UI/CrosshairBlueFill.png";
			break;
		case CrosshairState::OrangeFill:
			textureName = "Assets/Textures/UI/CrosshairOrangeFill.png";
			break;
		case CrosshairState::BothFill:
			textureName = "Assets/Textures/UI/CrosshairBothFill.png";
			break;
		default:
			break;
		}

		DrawTexture(shader, GetGame()->GetRenderer()->GetTexture(textureName));
	}
}

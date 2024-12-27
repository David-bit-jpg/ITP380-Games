#include "PacMan.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PacManMove.h"

PacMan::PacMan(class Game* game)
: Actor(game)
, mSpawnNode(nullptr)
{
	AnimatedSprite* asc = new AnimatedSprite(this, 150);

	// Create anim vectors
	std::vector<SDL_Texture*> rightAnim{game->GetTexture("Assets/PacMan/center.png"),
										game->GetTexture("Assets/PacMan/right0.png"),
										game->GetTexture("Assets/PacMan/right1.png")};
	std::vector<SDL_Texture*> leftAnim{game->GetTexture("Assets/PacMan/center.png"),
									   game->GetTexture("Assets/PacMan/left0.png"),
									   game->GetTexture("Assets/PacMan/left1.png")};
	std::vector<SDL_Texture*> upAnim{game->GetTexture("Assets/PacMan/center.png"),
									 game->GetTexture("Assets/PacMan/up0.png"),
									 game->GetTexture("Assets/PacMan/up1.png")};
	std::vector<SDL_Texture*> downAnim{game->GetTexture("Assets/PacMan/center.png"),
									   game->GetTexture("Assets/PacMan/down0.png"),
									   game->GetTexture("Assets/PacMan/down1.png")};

	std::vector<SDL_Texture*> deathAnim;
	for (int i = 0; i <= 10; i++)
	{
		std::string texName = "Assets/PacMan/death";
		texName += std::to_string(i);
		texName += ".png";
		deathAnim.emplace_back(game->GetTexture(texName));
	}

	// Add the animations
	asc->AddAnimation("right", rightAnim);
	asc->AddAnimation("left", leftAnim);
	asc->AddAnimation("up", upAnim);
	asc->AddAnimation("down", downAnim);
	asc->AddAnimation("death", deathAnim);

	// Default to right
	asc->SetAnimation("right");
	asc->SetIsPaused(true);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(20.0f, 20.0f);

	new PacManMove(this);
}

class PathNode* PacMan::GetPrevNode() const
{
	return GetComponent<PacManMove>()->GetPrevNode();
}

Vector2 PacMan::GetPointInFrontOf(float dist) const
{
	Vector2 dir = GetComponent<PacManMove>()->GetFacingDir();
	return (GetPosition() + dir * dist);
}

void PacMan::DoGameIntro()
{
	auto pm = GetComponent<PacManMove>();
	pm->StartRespawn(true);
}

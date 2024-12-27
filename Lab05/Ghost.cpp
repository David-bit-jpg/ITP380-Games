#include "Ghost.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "GhostAI.h"
#include <SDL2/SDL.h>
#include "PathNode.h"

Ghost::Ghost(class Game* game, Type type)
: Actor(game)
, mType(type)
, mSpawnNode(nullptr)
, mScatterNode(nullptr)
{
	AnimatedSprite* asc = new AnimatedSprite(this, 125);
	SetAnimatedSprite(asc);
	SetupMoveAnim("right");
	SetupMoveAnim("left");
	SetupMoveAnim("up");
	SetupMoveAnim("down");

	// Scared animations
	std::vector<SDL_Texture*> scaredBlue{
		game->GetTexture("Assets/GhostScared/blue0.png"),
		game->GetTexture("Assets/GhostScared/blue1.png"),
	};
	std::vector<SDL_Texture*> scaredFlash{
		game->GetTexture("Assets/GhostScared/white0.png"),
		game->GetTexture("Assets/GhostScared/white1.png"),
		game->GetTexture("Assets/GhostScared/blue0.png"),
		game->GetTexture("Assets/GhostScared/blue1.png"),
	};

	asc->AddAnimation("scared0", scaredBlue);
	asc->AddAnimation("scared1", scaredFlash);

	// Dead animations
	std::vector<SDL_Texture*> deadRight{
		game->GetTexture("Assets/GhostDead/right.png"),
	};
	std::vector<SDL_Texture*> deadLeft{
		game->GetTexture("Assets/GhostDead/left.png"),
	};
	std::vector<SDL_Texture*> deadUp{
		game->GetTexture("Assets/GhostDead/up.png"),
	};
	std::vector<SDL_Texture*> deadDown{
		game->GetTexture("Assets/GhostDead/down.png"),
	};

	asc->AddAnimation("deadright", deadRight);
	asc->AddAnimation("deadleft", deadLeft);
	asc->AddAnimation("deadup", deadUp);
	asc->AddAnimation("deaddown", deadDown);

	std::vector<SDL_Texture*> blank{
		game->GetTexture("Assets/GhostBlank.png"),
	};
	asc->AddAnimation("blank", blank);

	asc->SetAnimation("blank");

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(5.0f, 5.0f);
	SetCollisionComponent(cc);
	mAI = new GhostAI(this);
}

void Ghost::SetupMoveAnim(const std::string& name)
{
	AnimatedSprite* asc = GetComponent<AnimatedSprite>();

	std::string prefix = "Assets/";
	switch (mType)
	{
	case Blinky:
		prefix += "Blinky/";
		break;
	case Pinky:
		prefix += "Pinky/";
		break;
	case Inky:
		prefix += "Inky/";
		break;
	case Clyde:
	default:
		prefix += "Clyde/";
		break;
	}

	std::vector<SDL_Texture*> textures;
	textures.emplace_back(GetGame()->GetTexture(prefix + name + "0.png"));
	textures.emplace_back(GetGame()->GetTexture(prefix + name + "1.png"));

	asc->AddAnimation(name, textures);
}

void Ghost::Start()
{
	mAI->Start(mSpawnNode);
}

void Ghost::DebugDrawPath(struct SDL_Renderer* render)
{
	// Don't draw paths when paused
	if (GetState() == ActorState::Paused)
	{
		return;
	}

	// Set color based on type
	switch (mType)
	{
	case Blinky:
		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
		break;
	case Pinky:
		SDL_SetRenderDrawColor(render, 255, 184, 255, 255);
		break;
	case Inky:
		SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
		break;
	case Clyde:
	default:
		SDL_SetRenderDrawColor(render, 255, 184, 82, 255);
		break;
	}

	GhostAI* gm = GetComponent<GhostAI>();
	gm->DebugDrawPath(render);
}

void Ghost::Frighten()
{
	mAI->Frighten();
}

bool Ghost::IsFrightened() const
{
	return mAI->GetState() == GhostAI::Frightened;
}

bool Ghost::IsDead() const
{
	return mAI->GetState() == GhostAI::Dead;
}

void Ghost::Die()
{
	mGame->GetAudio()->PlaySound("EatGhost.wav");
	mAI->Die();
}

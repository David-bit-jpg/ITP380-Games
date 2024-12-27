#include "PacManMove.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include "Ghost.h"
#include "PacMan.h"
#include "PowerPellet.h"
#include "Pellet.h"

PacManMove::PacManMove(class Actor* owner)
: MoveComponent(owner)
, mPrevNode(nullptr)
, mAudio(owner->GetGame()->GetAudio())
{
	mChompSound = mAudio->PlaySound("ChompLoop.wav", true);
	mAudio->PauseSound(mChompSound);

	mSirenSound = mAudio->PlaySound("Siren.wav", true);
	mAudio->PauseSound(mSirenSound);

	mSirenFrightSound = mAudio->PlaySound("SirenFright.wav", true);
	mAudio->PauseSound(mSirenFrightSound);
}

void PacManMove::Update(float deltaTime)
{
	// In the process of respawning, don't
	// do anything else
	if (mRespawnTimer > 0.0f)
	{
		UpdateRespawn(deltaTime);
		return;
	}

	auto coll = mOwner->GetComponent<CollisionComponent>();
	auto game = mOwner->GetGame();

	// Check if we intersect with any ghosts
	for (auto g : game->GetGhosts())
	{
		if (!g->IsDead() && coll->Intersect(g->GetComponent<CollisionComponent>()))
		{
			if (g->IsFrightened())
			{
				// Kill ghost
				g->Die();
			}
			else
			{
				// Kill Pac-Man
				StartRespawn();
				return;
			}
		}
	}

	// Check if we intersect with a path node
	bool collided = false;
	for (auto p : game->GetPathNodes())
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			collided = true;
			mPrevNode = p;

			HandleNodeIntersect(p);
		}
	}

	// Check if we intersect with a power pellet
	for (auto p : game->GetPowerPellets())
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			for (auto g : game->GetGhosts())
			{
				g->Frighten();
			}
			p->SetState(ActorState::Destroy);
		}
	}

	// Check if we intersect with a pellet
	for (auto p : game->GetPellets())
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			p->SetState(ActorState::Destroy);
			mChompSoundTimer = CHOMP_TIME_PER;
		}
	}

	// See if we want to switch directions
	if (!Math::NearlyZero(mMoveDir.x) && !Math::NearlyZero(mInput.x))
	{
		mMoveDir.x = mInput.x;
	}
	else if (!Math::NearlyZero(mMoveDir.y) && !Math::NearlyZero(mInput.y))
	{
		mMoveDir.y = mInput.y;
	}

	// Update position
	Vector2 pos = mOwner->GetPosition();
	pos += mMoveDir * MAX_SPEED * deltaTime;

	// Turn smoothing
	// If we collided with a node, smooth us onto the opposite axis
	// of our movement
	// If we aren't moving at all, then smooth to the position of the node
	// (In case we can't move in the desired direction anymore)
	if (collided)
	{
		if (!Math::NearlyZero(mMoveDir.x))
		{
			pos.y = Math::Lerp(pos.y, mPrevNode->GetPosition().y, 0.25f);
		}
		else if (!Math::NearlyZero(mMoveDir.y))
		{
			pos.x = Math::Lerp(pos.x, mPrevNode->GetPosition().x, 0.25f);
		}
		else if (Math::NearlyZero(mMoveDir.Length()))
		{
			pos = Vector2::Lerp(pos, mPrevNode->GetPosition(), 0.25f);
		}
	}

	mOwner->SetPosition(pos);

	// Now update animations based on movement
	AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
	if (!Math::NearlyZero(mMoveDir.Length()))
	{
		asc->SetIsPaused(false);
		if (mMoveDir.y < 0.0f)
		{
			asc->SetAnimation("up");
			mFacingDir = Vector2::NegUnitY;
		}
		else if (mMoveDir.y > 0.0f)
		{
			asc->SetAnimation("down");
			mFacingDir = Vector2::UnitY;
		}
		else if (mMoveDir.x > 0.0f)
		{
			asc->SetAnimation("right");
			mFacingDir = Vector2::UnitX;
		}
		else
		{
			asc->SetAnimation("left");
			mFacingDir = Vector2::NegUnitX;
		}
	}
	else
	{
		asc->SetIsPaused(true);
	}

	// Update the ghost siren sound
	UpdateSounds(deltaTime);
}

void PacManMove::ProcessInput(const Uint8* keyState)
{
	mInput = Vector2::Zero;

	// Basic controller support (students won't implement this)
	SDL_GameController* controller = SDL_GameControllerOpen(0);
	const int STICK_THRESHOLD = 5000;
	if (controller)
	{
		int yValue = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
		int xValue = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (Math::Abs(static_cast<float>(yValue)) > Math::Abs(static_cast<float>(xValue)))
		{
			if (yValue > STICK_THRESHOLD)
			{
				mInput = Vector2::UnitY;
			}
			else if (yValue < -STICK_THRESHOLD)
			{
				mInput = Vector2::NegUnitY;
			}
		}
		else if (xValue > STICK_THRESHOLD)
		{
			mInput = Vector2::UnitX;
		}
		else if (xValue < -STICK_THRESHOLD)
		{
			mInput = Vector2::NegUnitX;
		}
	}

	if (keyState[SDL_SCANCODE_A])
	{
		mInput = Vector2::NegUnitX;
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		mInput = Vector2::UnitX;
	}
	else if (keyState[SDL_SCANCODE_W])
	{
		mInput = Vector2::NegUnitY;
	}
	else if (keyState[SDL_SCANCODE_S])
	{
		mInput = Vector2::UnitY;
	}
}

void PacManMove::HandleNodeIntersect(PathNode* p)
{
	// If we collide with a tunnel, need to teleport
	if (p == mOwner->GetGame()->GetTunnelLeft())
	{
		Vector2 newPos = p->GetPosition();
		newPos.x = mOwner->GetGame()->GetTunnelRight()->GetPosition().x;
		newPos.x -= 16.0f;
		mOwner->SetPosition(newPos);
		mPrevNode = mOwner->GetGame()->GetTunnelRight();
		p = mPrevNode;
	}
	else if (p == mOwner->GetGame()->GetTunnelRight())
	{
		Vector2 newPos = p->GetPosition();
		newPos.x = mOwner->GetGame()->GetTunnelLeft()->GetPosition().x;
		newPos.x += 16.0f;
		mOwner->SetPosition(newPos);
		mPrevNode = mOwner->GetGame()->GetTunnelLeft();
		p = mPrevNode;
	}

	// Figure out which directions of movement are allowed
	bool leftAllowed = false;
	bool rightAllowed = false;
	bool upAllowed = false;
	bool downAllowed = false;

	for (auto n : p->mAdjacent)
	{
		if (n->GetType() != PathNode::Ghost)
		{
			Vector2 diff = n->GetPosition() - p->GetPosition();
			if (diff.x > 1.0f)
			{
				rightAllowed = true;
			}
			else if (diff.x < -1.0f)
			{
				leftAllowed = true;
			}
			else if (diff.y > 1.0f)
			{
				downAllowed = true;
			}
			else if (diff.y < -1.0f)
			{
				upAllowed = true;
			}
		}
	}

	// Give priority to turns first
	if (!Math::NearlyZero(mMoveDir.x) && !Math::NearlyZero(mInput.y))
	{
		// Switch to up/down, if it's allowed
		if ((mInput.y < 0.0f && upAllowed) || (mInput.y > 0.0f && downAllowed))
		{
			mMoveDir.x = 0.0f;
			mMoveDir.y = mInput.y;
		}
	}
	else if (!Math::NearlyZero(mMoveDir.y) && !Math::NearlyZero(mInput.x))
	{
		// Switch to left/right, if it's allowed
		if ((mInput.x < 0.0f && leftAllowed) || (mInput.x > 0.0f && rightAllowed))
		{
			mMoveDir.x = mInput.x;
			mMoveDir.y = 0.0f;
		}
	}
	// Otherwise just move in input directions
	else if ((mInput.y < 0.0f && upAllowed) || (mInput.y > 0.0f && downAllowed))
	{
		mMoveDir.x = 0.0f;
		mMoveDir.y = mInput.y;
	}
	else if ((mInput.x < 0.0f && leftAllowed) || (mInput.x > 0.0f && rightAllowed))
	{
		mMoveDir.x = mInput.x;
		mMoveDir.y = 0.0f;
	}

	// Now also verify we can keep moving in the
	// current move direction
	if ((mMoveDir.x < 0.0f && !leftAllowed) || (mMoveDir.x > 0.0f && !rightAllowed))
	{
		mMoveDir.x = 0.0f;
	}
	if ((mMoveDir.y < 0.0f && !upAllowed) || (mMoveDir.y > 0.0f && !downAllowed))
	{
		mMoveDir.y = 0.0f;
	}
}

void PacManMove::StartRespawn(bool isGameIntro)
{
	if (!isGameIntro)
	{
		mRespawnTimer = RESPAWN_TIME;
	}
	else
	{
		mRespawnTimer = INTRO_TIME;
	}

	// Zero out move direction
	mMoveDir = Vector2::Zero;

	// Start playing death animation if not intro
	if (!isGameIntro)
	{
		AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
		asc->SetAnimation("death");
		asc->ResetAnimTimer();
		asc->SetIsPaused(false);

		mAudio->PlaySound("Death.wav");
	}

	// Pause all ghosts
	for (auto g : mOwner->GetGame()->GetGhosts())
	{
		g->SetState(ActorState::Paused);
	}

	mAudio->PauseSound(mChompSound);
	mAudio->PauseSound(mSirenSound);
	mAudio->PauseSound(mSirenFrightSound);
}

void PacManMove::UpdateRespawn(float deltaTime)
{
	mRespawnTimer -= deltaTime;
	if (mRespawnTimer <= 0.0f)
	{
		// Reset pac-man position and animation
		PathNode* spawn = static_cast<PacMan*>(mOwner)->GetSpawnNode();
		mOwner->SetPosition(spawn->GetPosition());

		AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
		asc->SetAnimation("right");
		asc->ResetAnimTimer();
		asc->SetIsPaused(true);

		// Reset all the ghosts
		for (auto g : mOwner->GetGame()->GetGhosts())
		{
			g->SetState(ActorState::Active);
			g->Start();
		}

		mAudio->ResumeSound(mSirenSound);
	}
}

void PacManMove::UpdateSounds(float deltaTime)
{
	mChompSoundTimer -= deltaTime;
	if (mChompSoundTimer < 0.0f)
	{
		mChompSoundTimer = 0.0f;
	}

	bool anyFrightened = false;
	bool anyAlive = false;
	for (auto g : mOwner->GetGame()->GetGhosts())
	{
		if (g->IsFrightened())
		{
			anyFrightened = true;
		}
		if (!g->IsDead())
		{
			anyAlive = true;
		}
	}

	if (anyFrightened)
	{
		mAudio->PauseSound(mSirenSound);
		mAudio->ResumeSound(mSirenFrightSound);

		mAudio->PauseSound(mChompSound);
	}
	else if (anyAlive)
	{
		mAudio->ResumeSound(mSirenSound);
		mAudio->PauseSound(mSirenFrightSound);

		if (mChompSoundTimer > 0.0f)
		{
			mAudio->ResumeSound(mChompSound);
		}
		else
		{
			mAudio->PauseSound(mChompSound);
		}
	}
	else
	{
		// They're all dead, no siren for now
		mAudio->PauseSound(mSirenSound);
		mAudio->ResumeSound(mSirenFrightSound);

		if (mChompSoundTimer > 0.0f)
		{
			mAudio->ResumeSound(mChompSound);
		}
		else
		{
			mAudio->PauseSound(mChompSound);
		}
	}
}

#include "SoldierAI.h"
#include "Actor.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>

SoldierAI::SoldierAI(Actor* owner)
: SpriteComponent(owner)
{
	SetIsVisible(false); // Uncomment to hide debug paths
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	mPatrolStart = start;
	mPatrolEnd = end;
	GetGame()->GetPathFinder()->CalculatePath(mPatrolStart, mPatrolEnd, mPath);
	mPath.pop_back();
	mPrev = mPatrolStart;
	mNext = mPath.back();
	mPath.pop_back();
}

void SoldierAI::Update(float deltaTime)
{
	if (!mIsStunned) //if it's not stunned(it can move)
	{
		GetSoldier()->GetAnimatedSprite()->SetIsPaused(false); //resume animation
		Vector2 direction = mNext->GetPosition() -
							mPrev->GetPosition(); //get vector for new moving direction
		float distance = Vector2::Distance(mNext->GetPosition(),
										   GetOwner()->GetPosition()); //find distance
		direction.Normalize();										   //normalize direction
		SetDirection(direction);
		GetOwner()->SetPosition(GetOwner()->GetPosition() +
								GetDirection() * SOLDIER_SPEED * deltaTime); //move
		if (distance <= THRESHOLD)											 //get to next node
		{
			GetOwner()->SetPosition(mNext->GetPosition());
			if (!mPath.empty()) //get next node
			{
				mPrev = mNext;
				mNext = mPath.back();
				mPath.pop_back();
			}
			else //if no nodes remaining
			{
				Setup(mPatrolEnd, mPatrolStart); //go back
			}
		}
		UpdateAnimation();
	}
	else
	{
		mStunnedTimer += deltaTime;
		GetSoldier()->GetAnimatedSprite()->SetIsPaused(true);
		if (mStunnedTimer >= STUN_DURATION)
		{
			mIsStunned = false;
			mStunnedTimer = 0.0f;
		}
	}
}
const void SoldierAI::UpdateAnimation() const
{
	Vector2 direction = GetDirection();
	if (direction.x == 1)
	{
		GetSoldier()->GetAnimatedSprite()->SetAnimation("WalkRight");
	}
	if (direction.x == -1)
	{
		GetSoldier()->GetAnimatedSprite()->SetAnimation("WalkLeft");
	}
	if (direction.y == -1)
	{
		GetSoldier()->GetAnimatedSprite()->SetAnimation("WalkUp");
	}
	if (direction.y == 1)
	{
		GetSoldier()->GetAnimatedSprite()->SetAnimation("WalkDown");
	}
}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

	// Draw from current position to next
	SDL_RenderDrawLine(renderer, static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer, static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}

	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer, static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}

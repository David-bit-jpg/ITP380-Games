#include "Actor.h"
#include "SDL2/SDL.h"
#include "VOTrigger.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Door.h"
VOTrigger::VOTrigger(Game* game)
: Actor(game)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLSIZE, COLLSIZE, COLLSIZE);
	mCollisionComponent = cc;
}

VOTrigger::~VOTrigger()
{
}
void VOTrigger::OnUpdate(float deltaTime)
{
	if (!GetGame()->GetPlayer()->GetComponent<HealthComponent>()->IsDead())
	{
		if (mIsActivated)
		{
			if (mIndex < mSounds.size())
			{
				if (GetGame()->GetAudio()->GetSoundState(mSound) == SoundState::Stopped)
				{
					mCurrentSoundName = mSounds[mIndex];
					mSound = GetGame()->GetAudio()->PlaySound(mCurrentSoundName);
					GetGame()->GetPlayer()->GetHUD()->ShowSubtitle(mSubtitles[mIndex]);
					mIndex++;
				}
			}
			else
			{
				if (GetGame()->GetAudio()->GetSoundState(mSound) == SoundState::Stopped)
				{
					GetGame()->GetPlayer()->GetHUD()->ShowSubtitle("");
					if (!mDoorName.empty())
					{
						for (Door* d : GetGame()->GetDoors())
						{
							if (d->GetName() == mDoorName && !d->GetOpened())
							{
								d->SetOpened(true);
								GetGame()->RemoveColliders(d);
							}
						}
					}
					if (!mNextLevel.empty())
					{
						GetGame()->SetNewLevel(mNextLevel);
					}
					SetState(ActorState::Destroy);
				}
			}
		}
		else
		{
			if (mCollisionComponent->Intersect(
					GetGame()->GetPlayer()->GetComponent<CollisionComponent>()))
			{
				mIsActivated = true;
				mCurrentSoundName = mSounds[mIndex];
				mSound = GetGame()->GetAudio()->PlaySound(mCurrentSoundName);
				GetGame()->GetPlayer()->GetHUD()->ShowSubtitle(mSubtitles[mIndex]);
				mIndex++;
			}
		}
	}
	else
	{
		if (GetGame()->GetAudio()->GetSoundState(mSound) != SoundState::Stopped)
		{
			GetGame()->GetAudio()->StopSound(mSound);
		}
	}
}
void VOTrigger::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							   const Vector2& relativeMouse)
{
	bool fPressed = keyState[SDL_SCANCODE_F]; //current state
	if (!mIsFPressed && fPressed)
	{
		if (GetGame()->GetAudio()->GetSoundState(mSound) != SoundState::Stopped)
		{
			GetGame()->GetAudio()->StopSound(mSound);
		}
	}
	mIsFPressed = fPressed;
}

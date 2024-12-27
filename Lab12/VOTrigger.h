#pragma once
#include "Actor.h"
#include "SDL2/SDL.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include <vector>
class MeshComponent;
class CollisionComponent;
class Game;
class VOTrigger : public Actor
{
private:
	CollisionComponent* mCollisionComponent = nullptr;
	std::string mDoorName;
	std::string mNextLevel;
	std::vector<std::string> mSounds;
	std::vector<std::string> mSubtitles;
	std::string mCurrentSoundName;
	bool mIsActivated = false;
	SoundHandle mSound;
	int mIndex = 0;
	float mIsFPressed = false;

	float const COLLSIZE = 1.0f;

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
						const Vector2& relativeMouse) override;

public:
	VOTrigger(Game* game);
	~VOTrigger();

	void SetDoorName(std::string s) { mDoorName = s; }
	void SetNextLevel(std::string s) { mNextLevel = s; }
	void SetSounds(std::vector<std::string> s) { mSounds = s; }
	void SetSubtitles(std::vector<std::string> s) { mSubtitles = s; }
};

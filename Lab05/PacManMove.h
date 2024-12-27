#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "AudioSystem.h"

class PacManMove : public MoveComponent
{
public:
	PacManMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

	class PathNode* GetPrevNode() const { return mPrevNode; }

	Vector2 GetFacingDir() const { return mFacingDir; }

	void StartRespawn(bool isGameIntro = false);

	void UpdateSounds(float deltaTime);

private:
	void HandleNodeIntersect(class PathNode* p);
	void UpdateRespawn(float deltaTime);

	const float MAX_SPEED = 100.0f;
	const float RESPAWN_TIME = 1.1f;
	const float INTRO_TIME = 4.0f;
	const float CHOMP_TIME_PER = 0.2f;

	Vector2 mMoveDir = Vector2::Zero;
	Vector2 mFacingDir = Vector2::UnitX;
	Vector2 mInput = Vector2::Zero;

	float mRespawnTimer = 0.0f;

	class PathNode* mPrevNode;

	AudioSystem* mAudio;
	SoundHandle mChompSound;
	SoundHandle mSirenSound;
	SoundHandle mSirenFrightSound;
	float mChompSoundTimer = 0.0f;
};

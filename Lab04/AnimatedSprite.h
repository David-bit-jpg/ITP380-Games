#pragma once
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

class AnimatedSprite : public SpriteComponent
{
public:
	// (Lower draw order corresponds with further back)
	AnimatedSprite(class Actor* owner, int drawOrder = 100);

	// Update the AnimatedSprite
	void Update(float deltaTime) override;

	// Add an animation of the corresponding name to the animation map
	void AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images);

	// Set the current active animation
	void SetAnimation(const std::string& name)
	{
		mAnimName = name;
		ForceUpdate();
	}

	// Get the name of the currently-playing animation
	const std::string& GetAnimName() const { return mAnimName; }

	// Reset the animation back to frame/time 0
	void ResetAnimTimer()
	{
		mAnimTimer = 0.0f;
		ForceUpdate();
	}

	// Use to pause/unpause the animation
	void SetIsPaused(bool pause) { mIsPaused = pause; }

	// Use to change the FPS of the animation
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

	// Use to get the current FPS of the animation
	float GetAnimFPS() const { return mAnimFPS; }

	// Use to get the total duration of the animation of he specified name
	float GetAnimDuration(const std::string& name) { return mAnims[name].size() / mAnimFPS; }

protected:
	void ForceUpdate() { Update(0.0f); }

	// Map of animation name to vector of textures corresponding to the animation
	std::unordered_map<std::string, std::vector<SDL_Texture*>> mAnims;

	// Name of current animation
	std::string mAnimName;

	// Whether or not the animation is paused (defaults to false)
	bool mIsPaused = false;

	// Tracks current elapsed time in animation
	float mAnimTimer = 0.0f;

	// The frames per second the animation should run at
	float mAnimFPS = 10.0f;
};

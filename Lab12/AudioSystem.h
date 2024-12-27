#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "SDL2/SDL_stdinc.h"
#include "Actor.h"
class Game;
class Actor;
// SoundHandles are used to operate on active sounds
class SoundHandle
{
public:
	// Returns true if this is an active sound handle
	bool IsValid() const { return mID != 0; }

	// Resets to inactive sound handle
	void Reset() { mID = 0; }

	// Increments for convenience
	SoundHandle& operator++()
	{
		mID++;
		return *this;
	}

	SoundHandle operator++(int)
	{
		SoundHandle temp(*this);
		mID++;
		return temp;
	}

	const char* GetDebugStr() const
	{
		static std::string tempStr;
		tempStr = std::to_string(mID);
		return tempStr.c_str();
	}

	// Boolean checks
	bool operator==(const SoundHandle& rhs) const { return mID == rhs.mID; }
	bool operator!=(const SoundHandle& rhs) const { return mID != rhs.mID; }
	bool operator<(const SoundHandle& rhs) const { return mID < rhs.mID; }
	bool operator<=(const SoundHandle& rhs) const { return mID <= rhs.mID; }
	bool operator>(const SoundHandle& rhs) const { return mID > rhs.mID; }
	bool operator>=(const SoundHandle& rhs) const { return mID >= rhs.mID; }

	static SoundHandle Invalid;

private:
	unsigned int mID = 0;
};

// Used to get information about state of sound
enum class SoundState
{
	Stopped,
	Playing,
	Paused
};

// Manages playing audio through SDL_mixer
class AudioSystem
{
public:
	// Create the AudioSystem with specified number of channels
	// (Defaults to 8 channels)
	AudioSystem(class Game* game, int numChannels = 8);
	// Destroy the AudioSystem
	~AudioSystem();

	// Updates the status of all the active sounds every frame
	void Update(float deltaTime);
	// Input for debugging purposes
	void ProcessInput(const Uint8* keyState);

	// Plays the sound with the specified name and loops if looping is true
	// Returns the SoundHandle which is used to perform any other actions on the
	// sound when active
	// NOTE: The soundName is without the "Assets/Sounds/" part of the file
	//       For example, pass in "ChompLoop.wav" rather than
	//       "Assets/Sounds/ChompLoop.wav".
	SoundHandle PlaySound(const std::string& soundName, bool looping = false,
						  class Actor* actor = nullptr, bool stopOnActorRemove = true,
						  int fadeTimeMS = 0);

	// Stops the sound if it is currently playing
	void StopSound(SoundHandle sound, int fadeTimeMS = 0);

	// Pauses the sound if it is currently playing
	void PauseSound(SoundHandle sound);

	// Resumes the sound if it is currently paused
	void ResumeSound(SoundHandle sound);

	// Returns the current state of the sound
	SoundState GetSoundState(SoundHandle sound);

	// Stops all sounds on all channels
	void StopAllSounds();

	// Cache all sounds under Assets/Sounds
	void CacheAllSounds();

	// Used to preload the sound data of a sound
	// NOTE: The soundName is without the "Assets/Sounds/" part of the file
	//       For example, pass in "ChompLoop.wav" rather than
	//       "Assets/Sounds/ChompLoop.wav".
	void CacheSound(const std::string& soundName);

	void RemoveActor(class Actor* actor);

private:
	// If the sound is already loaded, returns Mix_Chunk from the map.
	// Otherwise, will attempt to load the file and save it in the map.
	// Returns nullptr if sound is not found.
	// NOTE: The soundName is without the "Assets/Sounds/" part of the file
	//       For example, pass in "ChompLoop.wav" rather than
	//       "Assets/Sounds/ChompLoop.wav".
	struct Mix_Chunk* GetSound(const std::string& soundName);
	// Internal struct used to track the properties of active sound handles
	struct HandleInfo
	{
		std::string mSoundName;
		int mChannel = -1;
		bool mIsLooping = false;
		bool mIsPaused = false;
		class Actor* mActor = nullptr;
		bool mStopOnActorRemove = true;
	};

	// Tracks the active SoundHandle for each channel
	// An Invalid SoundHandle means the channel is free, otherwise
	// it's an active handle.
	std::vector<SoundHandle> mChannels;

	// Maps all the active SoundHandles to their HandleInfo
	std::map<SoundHandle, HandleInfo> mHandleMap;

	// Map to store the Mix_Chunk data for all the files
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// Used to track the last audio handle value used
	// Will increment prior to playing a new sound
	SoundHandle mLastHandle;

	// Used for debug input in ProcessInput
	bool mLastDebugKey = false;

	class Game* mGame;

	// Map for actors to their handles
	std::unordered_map<class Actor*, std::set<SoundHandle>> mActorMap;

	int CalculateVolume(class Actor* actor, class Actor* listener) const;

	int const MAX_VOLUME = 128;
	float const MAX_DIST = 600.0f;
	float const MIN_DIST = 25.0f;
};

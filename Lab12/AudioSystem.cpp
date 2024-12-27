#include "AudioSystem.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <filesystem>
#include "Math.h"
#include "Actor.h"
#include "Game.h"

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(class Game* game, int numChannels)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(numChannels);
	mChannels.resize(numChannels);
	mGame = game;
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem()
{
	for (auto& sound : mSounds)
	{
		Mix_FreeChunk(sound.second);
	}
	mSounds.clear();
	Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime)
{
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		if (mChannels[i].IsValid())
		{
			if (!Mix_Playing(static_cast<int>(i)))
			{
				auto it = mHandleMap.find(mChannels[i]);
				if (it != mHandleMap.end())
				{
					if (it->second.mActor)
					{
						int volumn = CalculateVolume(it->second.mActor, mGame->GetPlayer());
						Mix_Volume(static_cast<int>(i), volumn);
						auto iter = mActorMap.find(it->second.mActor);
						if (iter != mActorMap.end())
						{
							auto ite = iter->second.find(mChannels[i]);
							if (ite != iter->second.end())
							{
								iter->second.erase(ite);
							}
						}
					}
				}
				mHandleMap.erase(mChannels[i]);
				mChannels[i].Reset();
			}
		}
	}
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping, class Actor* actor,
								   bool stopOnActorRemove, int fadeTimeMS)
{
	Mix_Chunk* mCurrentSound = GetSound(soundName);
	if (!mCurrentSound)
	{
		SDL_Log("[AudioSystem] PlaySound couldn't find sound for %s", soundName.c_str());
		return SoundHandle::Invalid;
	}

	int availableChannel = -1;
	for (int i = 0; i < mChannels.size(); i++)
	{
		if (!mChannels[i].IsValid())
		{
			availableChannel = i;
			break;
		}
	}
	if (availableChannel == -1)
	{
		std::string oldName;
		for (auto& i : mHandleMap)
		{
			if (i.second.mSoundName == soundName)
			{
				availableChannel = i.second.mChannel;
				oldName = i.second.mSoundName;
				mHandleMap.erase(mChannels[availableChannel]);
				break;
			}
		}
		if (availableChannel == -1)
		{
			for (auto& i : mHandleMap)
			{
				if (!i.second.mIsLooping)
				{
					availableChannel = i.second.mChannel;
					oldName = i.second.mSoundName;
					mHandleMap.erase(mChannels[availableChannel]);
					break;
				}
			}
		}
		if (availableChannel == -1)
		{
			for (auto& i : mHandleMap)
			{
				availableChannel = i.second.mChannel;
				oldName = i.second.mSoundName;
				mHandleMap.erase(mChannels[availableChannel]);
				break;
			}
		}
		SDL_Log("[AudioSystem] PlaySound ran out of channels playing %s! Stopping %s",
				soundName.c_str(), oldName.c_str());
	}
	mLastHandle++;
	HandleInfo info{soundName, availableChannel, looping, false, actor, stopOnActorRemove};
	mHandleMap[mLastHandle] = info;
	mChannels[availableChannel] = mLastHandle;
	if (fadeTimeMS > 0)
	{
		Mix_FadeInChannel(availableChannel, mCurrentSound, looping ? -1 : 0, fadeTimeMS);
	}
	else
	{
		Mix_PlayChannel(availableChannel, mCurrentSound, looping ? -1 : 0);
		int volumn = CalculateVolume(actor, mGame->GetPlayer());
		Mix_Volume(availableChannel, volumn);
	}
	if (actor)
	{
		mActorMap[actor].emplace(mLastHandle);
	}
	return mLastHandle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound, int fadeTimeMS)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] StopSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}
	if (fadeTimeMS > 0)
	{
		Mix_FadeOutChannel(it->second.mChannel, fadeTimeMS);
	}
	else
	{
		Mix_HaltChannel(it->second.mChannel);
		mChannels[it->second.mChannel].Reset();
		mHandleMap.erase(it);
	}
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] PauseSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}
	if (!it->second.mIsPaused)
	{
		Mix_Pause(it->second.mChannel);
		it->second.mIsPaused = true;
	}
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] ResumeSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}
	if (it->second.mIsPaused)
	{
		Mix_Resume(it->second.mChannel);
		it->second.mIsPaused = false;
	}
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound)
{
	for (auto s : mHandleMap)
	{
		if (s.first == sound)
		{
			return s.second.mIsPaused ? SoundState::Paused : SoundState::Playing;
		}
	}
	return SoundState::Stopped;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds()
{
	Mix_HaltChannel(-1);
	for (SoundHandle channel : mChannels)
	{
		channel.Reset();
	}
	mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds()
{
#ifndef __clang_analyzer__
	std::error_code ec{};
	for (const auto& rootDirEntry : std::filesystem::directory_iterator{"Assets/Sounds", ec})
	{
		std::string extension = rootDirEntry.path().extension().string();
		if (extension == ".ogg" || extension == ".wav")
		{
			std::string fileName = rootDirEntry.path().stem().string();
			fileName += extension;
			CacheSound(fileName);
		}
	}
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string& soundName)
{
	GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk* AudioSystem::GetSound(const std::string& soundName)
{
	std::string fileName = "Assets/Sounds/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8* keyState)
{
	// Debugging code that outputs all active sounds on leading edge of period key
	if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey)
	{
		SDL_Log("[AudioSystem] Active Sounds:");
		for (size_t i = 0; i < mChannels.size(); i++)
		{
			if (mChannels[i].IsValid())
			{
				auto iter = mHandleMap.find(mChannels[i]);
				if (iter != mHandleMap.end())
				{
					HandleInfo& hi = iter->second;
					SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
							static_cast<unsigned>(i), mChannels[i].GetDebugStr(),
							hi.mSoundName.c_str(), hi.mIsLooping, hi.mIsPaused);
				}
				else
				{
					SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
							mChannels[i].GetDebugStr());
				}
			}
		}
	}

	mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}

void AudioSystem::RemoveActor(class Actor* actor)
{
	auto iter = mActorMap.find(actor);
	if (iter != mActorMap.end())
	{
		for (SoundHandle sh : iter->second)
		{
			auto it = mHandleMap.find(sh);
			if (it != mHandleMap.end())
			{
				it->second.mActor = nullptr;
				if (it->second.mStopOnActorRemove)
				{
					if (Mix_Playing(it->second.mChannel))
					{
						Mix_HaltChannel(it->second.mChannel);
					}
				}
			}
		}
		mActorMap.erase(iter);
	}
}
int AudioSystem::CalculateVolume(Actor* actor, Actor* listener) const
{
	if (!actor || !listener)
	{
		return MAX_VOLUME;
	}
	float distance = Vector3::Distance(actor->GetWorldPosition(), listener->GetWorldPosition());
	if (distance >= MAX_DIST)
	{
		return 0;
	}
	else if (distance <= MIN_DIST)
	{
		return MAX_VOLUME;
	}
	else
	{
		float percent = 1 - (distance - MIN_DIST) / (MAX_DIST - MIN_DIST);
		return static_cast<int>(percent * MAX_VOLUME);
	}
	return 0;
}
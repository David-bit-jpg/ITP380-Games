#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL_scancode.h>
#include <rapidjson/document.h>
#include "Math.h"

class InputReplay
{
public:
	InputReplay(class Game* game);

	void StartRecording(const std::string& levelName);
	void StopRecording();

	void StartPlayback(const std::string& levelName, bool enableValidation = true);
	void StopPlayback();

	void RecordInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse);
	void Update(float deltaTime);

	void InputPlayback(const Uint8*& keyState, Uint32& mouseButtons, Vector2& relativeMouse);

private:
	void ResetPlayer();
	const Vector3& GetPlayerPosition() const;
	const Vector3& GetPlayerVelocity() const;
	const Vector3& GetPlayerAcceleration() const;
	float GetPlayerYaw() const;
	float GetPlayerPitch() const;

	class Actor* GetBluePortal();
	class Actor* GetOrangePortal();

	std::string mLevelName;
	std::map<SDL_Scancode, bool> mKeyStates;

	struct PlayerInfo
	{
		Vector3 mPosition;
		Vector3 mVelocity;
		Vector3 mAcceleration;
		float mYaw = 0.0f;
		float mPitch = 0.0f;

		friend PlayerInfo operator-(const PlayerInfo& a, const PlayerInfo& b)
		{
			PlayerInfo result = a;
			result.mPosition -= b.mPosition;
			result.mVelocity -= b.mVelocity;
			result.mAcceleration -= b.mAcceleration;
			result.mYaw -= b.mYaw;
			result.mPitch -= b.mPitch;
			return result;
		}

		PlayerInfo& operator+=(const PlayerInfo& b)
		{
			mPosition += b.mPosition;
			mVelocity += b.mVelocity;
			mAcceleration += b.mAcceleration;
			mYaw += b.mYaw;
			mPitch += b.mPitch;
			return *this;
		}
	};
	PlayerInfo mPlayerInfo;

	struct PortalInfo
	{
		Vector3 mPosition;
		Quaternion mQuat;
		float mWidth = 0.0f;
		float mHeight = 0.0f;
		float mDepth = 0.0f;
		bool mExists = false;
		bool mUpdated = false;
	};
	PortalInfo mBluePortalInfo;
	PortalInfo mOrangePortalInfo;

	void FillPortalInfo(class Actor* portal, PortalInfo& outInfo);
	void WritePortalJSON(const PortalInfo& info, rapidjson::MemoryPoolAllocator<>& allocator,
						 rapidjson::Value& outJSON);
	void ReadPortalJSON(const rapidjson::Value& value, PortalInfo& outInfo);

	class Actor* mBluePortal = nullptr;
	class Actor* mOrangePortal = nullptr;

	struct InputEvent
	{
		float mTimestamp = -1.0f;
		std::map<SDL_Scancode, bool> mKeyChanges;
		Uint32 mMouseButtons = 0;
		Vector2 mRelativeMouse;

		PlayerInfo mPlayerDelta;
		bool mHasPlayerDelta = false;

		PortalInfo mBluePortal;
		PortalInfo mOrangePortal;
	};
	std::vector<InputEvent> mEvents;

	Uint8 mPlaybackKeys[SDL_NUM_SCANCODES];

	bool mIsRecording = false;
	bool mIsInPlayback = false;

	float mLastTimestamp = 0.0f;
	int mCurrentPlaybackIndex = -1;
	bool mEnableValidation = false;

	class Game* mGame;
};

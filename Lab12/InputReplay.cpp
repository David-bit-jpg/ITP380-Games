#include "InputReplay.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include "SDL2/SDL_log.h"
#include "Game.h"
#include "Actor.h"
#include "CollisionComponent.h"

namespace
{
	std::string ConvertLevelToReplay(const std::string& levelName)
	{
		size_t dotLoc = levelName.find_first_of('/');
		std::string firstHalf = levelName.substr(0, dotLoc + 1);
		std::string secondHalf = levelName.substr(dotLoc);
		return firstHalf + "Replays/" + secondHalf;
	}

	void ValidateVector(const char* description, const Vector3& expected, const Vector3& actual)
	{
		if (!Math::NearlyEqual(expected, actual, 0.5f))
		{
			SDL_LogWarn(0, "%s mismatch.\nExpected: (%f, %f, %f)\nActual:   (%f, %f, %f)",
						description, expected.x, expected.y, expected.z, actual.x, actual.y,
						actual.z);
			SDL_assert(
				false &&
				"InputReplay validation failed, check output log for details or break for debugger");
		}
	}

	void ValidateQuat(const char* description, const Quaternion& expected, const Quaternion& actual)
	{
		if (!Math::NearlyEqual(expected, actual))
		{
			SDL_LogWarn(0, "%s mismatch.\nExpected: (%f, %f, %f, %f)\nActual:   (%f, %f, %f, %f)",
						description, expected.x, expected.y, expected.z, expected.w, actual.x,
						actual.y, actual.z, actual.w);
			SDL_assert(
				false &&
				"InputReplay validation failed, check output log for details or break for debugger");
		}
	}

	void ValidateFloat(const char* description, float expected, float actual)
	{
		if (!Math::NearlyEqual(expected, actual))
		{
			SDL_LogWarn(0, "%s mismatch.\nExpected: (%f)\nActual:   (%f)", description, expected,
						actual);
			SDL_assert(
				false &&
				"InputReplay validation failed, check output log for details or break for debugger");
		}
	}
} // namespace

InputReplay::InputReplay(class Game* game)
: mGame(game)
{
	mKeyStates.emplace(SDL_SCANCODE_W, false);
	mKeyStates.emplace(SDL_SCANCODE_A, false);
	mKeyStates.emplace(SDL_SCANCODE_S, false);
	mKeyStates.emplace(SDL_SCANCODE_D, false);
	mKeyStates.emplace(SDL_SCANCODE_R, false);
	mKeyStates.emplace(SDL_SCANCODE_F, false);
	mKeyStates.emplace(SDL_SCANCODE_F5, false);
	mKeyStates.emplace(SDL_SCANCODE_SPACE, false);

	std::memset(mPlaybackKeys, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
}

void InputReplay::StartRecording(const std::string& levelName)
{
	if (!mIsRecording && !mIsInPlayback)
	{
		mIsRecording = true;
		mLastTimestamp = 0.0f;
		mEvents.clear();
		mPlayerInfo = PlayerInfo();
		mBluePortal = nullptr;
		mOrangePortal = nullptr;
		mLevelName = levelName;
	}
}

void InputReplay::StopRecording()
{
	if (mIsRecording)
	{
		mIsRecording = false;

		// Write out the JSON
		rapidjson::Document doc;
		auto& allocator = doc.GetAllocator();
		doc.SetArray();
		for (const auto& event : mEvents)
		{
			rapidjson::Value entry;
			entry.SetObject();
			entry.AddMember("t", event.mTimestamp, allocator);

			rapidjson::Value mouse;
			mouse.SetObject();
			mouse.AddMember("x", event.mRelativeMouse.x, allocator);
			mouse.AddMember("y", event.mRelativeMouse.y, allocator);
			mouse.AddMember("b", event.mMouseButtons, allocator);
			entry.AddMember("m", mouse, allocator);

			rapidjson::Value keysV;
			keysV.SetArray();
			for (const auto& keyPair : event.mKeyChanges)
			{
				rapidjson::Value keyV;
				keyV.SetObject();
				keyV.AddMember("k", static_cast<int>(keyPair.first), allocator);
				keyV.AddMember("v", keyPair.second, allocator);
				keysV.PushBack(keyV, allocator);
			}
			entry.AddMember("k", keysV, allocator);

			if (event.mHasPlayerDelta)
			{
				rapidjson::Value player;
				player.SetObject();

				rapidjson::Value playerPos;
				playerPos.SetArray();
				playerPos.PushBack(event.mPlayerDelta.mPosition.x, allocator);
				playerPos.PushBack(event.mPlayerDelta.mPosition.y, allocator);
				playerPos.PushBack(event.mPlayerDelta.mPosition.z, allocator);
				player.AddMember("p", playerPos, allocator);

				rapidjson::Value playerVel;
				playerVel.SetArray();
				playerVel.PushBack(event.mPlayerDelta.mVelocity.x, allocator);
				playerVel.PushBack(event.mPlayerDelta.mVelocity.y, allocator);
				playerVel.PushBack(event.mPlayerDelta.mVelocity.z, allocator);
				player.AddMember("v", playerVel, allocator);

				rapidjson::Value playerAccel;
				playerAccel.SetArray();
				playerAccel.PushBack(event.mPlayerDelta.mAcceleration.x, allocator);
				playerAccel.PushBack(event.mPlayerDelta.mAcceleration.y, allocator);
				playerAccel.PushBack(event.mPlayerDelta.mAcceleration.z, allocator);
				player.AddMember("a", playerAccel, allocator);

				player.AddMember("y", event.mPlayerDelta.mYaw, allocator);
				player.AddMember("pi", event.mPlayerDelta.mPitch, allocator);

				entry.AddMember("p", player, allocator);
			}

			if (event.mBluePortal.mUpdated)
			{
				rapidjson::Value portal;
				portal.SetObject();

				WritePortalJSON(event.mBluePortal, allocator, portal);
				entry.AddMember("bp", portal, allocator);
			}

			if (event.mOrangePortal.mUpdated)
			{
				rapidjson::Value portal;
				portal.SetObject();

				WritePortalJSON(event.mOrangePortal, allocator, portal);
				entry.AddMember("op", portal, allocator);
			}

			doc.PushBack(entry, allocator);
		}

		std::string replayFile = ConvertLevelToReplay(mLevelName);
		std::ofstream file(replayFile);
		rapidjson::OStreamWrapper osw(file);

		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
		doc.Accept(writer);
	}
}

void InputReplay::StartPlayback(const std::string& levelName, bool enableValidation /*= true*/)
{
	if (!mIsInPlayback && !mIsRecording)
	{
		ResetPlayer();

		mEnableValidation = enableValidation;
		mPlayerInfo = PlayerInfo();
		mBluePortal = nullptr;
		mOrangePortal = nullptr;
		mBluePortalInfo = PortalInfo();
		mOrangePortalInfo = PortalInfo();

		mIsInPlayback = true;
		mLastTimestamp = 0.0f;
		std::memset(mPlaybackKeys, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
		mEvents.clear();
		mCurrentPlaybackIndex = -1;

		// Read in the JSON
		std::string replayFile = ConvertLevelToReplay(levelName);
		std::ifstream file(replayFile);
		if (!file.is_open())
		{
			SDL_Log("Assets/Replay.json file not found");
			return;
		}

		std::stringstream fileStream;
		fileStream << file.rdbuf();
		std::string contents = fileStream.str();
		rapidjson::StringStream jsonStr(contents.c_str());
		rapidjson::Document doc;
		doc.ParseStream(jsonStr);

		if (!doc.IsArray())
		{
			SDL_Log("Assets/Replay.json file is invalid");
			return;
		}

		mEvents.reserve(doc.Size());
		for (rapidjson::SizeType i = 0; i < doc.Size(); i++)
		{
			const rapidjson::Value& iter = doc[i];

			InputEvent event;
			event.mTimestamp = iter["t"].GetFloat();
			event.mRelativeMouse.x = iter["m"]["x"].GetFloat();
			event.mRelativeMouse.y = iter["m"]["y"].GetFloat();
			event.mMouseButtons = iter["m"]["b"].GetUint();

			const rapidjson::Value& keys = iter["k"];
			for (rapidjson::SizeType j = 0; j < keys.Size(); j++)
			{
				event.mKeyChanges.emplace(static_cast<SDL_Scancode>(keys[j]["k"].GetInt()),
										  keys[j]["v"].GetBool());
			}

			if (iter.HasMember("p"))
			{
				event.mPlayerDelta.mPosition.x = iter["p"]["p"][0].GetFloat();
				event.mPlayerDelta.mPosition.y = iter["p"]["p"][1].GetFloat();
				event.mPlayerDelta.mPosition.z = iter["p"]["p"][2].GetFloat();

				event.mPlayerDelta.mVelocity.x = iter["p"]["v"][0].GetFloat();
				event.mPlayerDelta.mVelocity.y = iter["p"]["v"][1].GetFloat();
				event.mPlayerDelta.mVelocity.z = iter["p"]["v"][2].GetFloat();

				event.mPlayerDelta.mAcceleration.x = iter["p"]["a"][0].GetFloat();
				event.mPlayerDelta.mAcceleration.y = iter["p"]["a"][1].GetFloat();
				event.mPlayerDelta.mAcceleration.z = iter["p"]["a"][2].GetFloat();

				event.mPlayerDelta.mYaw = iter["p"]["y"].GetFloat();
				event.mPlayerDelta.mPitch = iter["p"]["pi"].GetFloat();
			}

			if (iter.HasMember("bp"))
			{
				ReadPortalJSON(iter["bp"], event.mBluePortal);
			}

			if (iter.HasMember("op"))
			{
				ReadPortalJSON(iter["op"], event.mOrangePortal);
			}

			mEvents.emplace_back(event);
		}
	}
}

void InputReplay::StopPlayback()
{
	if (mIsInPlayback)
	{
		mIsInPlayback = false;
	}
}

void InputReplay::RecordInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{
	if (mIsRecording)
	{
		InputEvent event;
		for (auto& key : mKeyStates)
		{
			if (static_cast<bool>(keyState[key.first]) != key.second)
			{
				event.mTimestamp = mLastTimestamp;
				event.mKeyChanges.emplace(key.first, keyState[key.first]);
				key.second = keyState[key.first];
			}
		}

		if (!Math::NearlyZero(relativeMouse.Length()))
		{
			event.mTimestamp = mLastTimestamp;
			event.mRelativeMouse = relativeMouse;
		}

		if (mouseButtons != 0)
		{
			event.mTimestamp = mLastTimestamp;
			event.mMouseButtons = mouseButtons;
		}

		PlayerInfo newPlayerInfo;
		newPlayerInfo.mPosition = GetPlayerPosition();
		newPlayerInfo.mVelocity = GetPlayerVelocity();
		newPlayerInfo.mAcceleration = GetPlayerAcceleration();
		newPlayerInfo.mYaw = GetPlayerYaw();
		newPlayerInfo.mPitch = GetPlayerPitch();

		if (!Math::NearlyEqual(mPlayerInfo.mPosition, newPlayerInfo.mPosition) ||
			!Math::NearlyEqual(mPlayerInfo.mVelocity, newPlayerInfo.mVelocity) ||
			!Math::NearlyEqual(mPlayerInfo.mAcceleration, newPlayerInfo.mAcceleration) ||
			!Math::NearlyEqual(mPlayerInfo.mYaw, newPlayerInfo.mYaw) ||
			!Math::NearlyEqual(mPlayerInfo.mPitch, newPlayerInfo.mPitch))
		{
			event.mTimestamp = mLastTimestamp;
			event.mPlayerDelta = newPlayerInfo - mPlayerInfo;
			event.mHasPlayerDelta = true;
			mPlayerInfo = newPlayerInfo;
		}

		Actor* bluePortal = GetBluePortal();
		if (bluePortal != mBluePortal)
		{
			event.mTimestamp = mLastTimestamp;
			mBluePortal = bluePortal;
			FillPortalInfo(mBluePortal, event.mBluePortal);
		}

		Actor* orangePortal = GetOrangePortal();
		if (orangePortal != mOrangePortal)
		{
			event.mTimestamp = mLastTimestamp;
			mOrangePortal = orangePortal;
			FillPortalInfo(mOrangePortal, event.mOrangePortal);
		}

		if (event.mTimestamp >= 0.0f)
		{
			mEvents.emplace_back(event);
		}
	}
}

void InputReplay::Update(float deltaTime)
{
	if (mIsRecording || mIsInPlayback)
	{
		mLastTimestamp += deltaTime;
	}
}

void InputReplay::InputPlayback(const Uint8*& keyState, Uint32& mouseButtons,
								Vector2& relativeMouse)
{
	if (mIsInPlayback)
	{
		keyState = mPlaybackKeys;
		relativeMouse = Vector2::Zero;
		mouseButtons = 0;
		while (static_cast<size_t>(mCurrentPlaybackIndex + 1) < mEvents.size() &&
			   mLastTimestamp >= mEvents[mCurrentPlaybackIndex + 1].mTimestamp)
		{
			mCurrentPlaybackIndex++;
			const InputEvent& event = mEvents[mCurrentPlaybackIndex];
			relativeMouse += event.mRelativeMouse;
			mouseButtons = event.mMouseButtons;

			for (const auto& key : event.mKeyChanges)
			{
				mPlaybackKeys[key.first] = key.second;
			}

			mPlayerInfo += event.mPlayerDelta;

			if (event.mBluePortal.mUpdated)
			{
				mBluePortalInfo = event.mBluePortal;
				mBluePortal = GetBluePortal();
			}

			if (event.mOrangePortal.mUpdated)
			{
				mOrangePortalInfo = event.mOrangePortal;
				mOrangePortal = GetOrangePortal();
			}
		}

		if (mEnableValidation)
		{
			ValidateVector("Player acceleration", mPlayerInfo.mAcceleration,
						   GetPlayerAcceleration());
			ValidateVector("Player velocity", mPlayerInfo.mVelocity, GetPlayerVelocity());
			ValidateVector("Player position", mPlayerInfo.mPosition, GetPlayerPosition());
			ValidateFloat("Player yaw", mPlayerInfo.mYaw, GetPlayerYaw());
			ValidateFloat("Player pitch", mPlayerInfo.mPitch, GetPlayerPitch());

			if (mBluePortal != GetBluePortal())
			{
				SDL_LogWarn(0, "Blue portal actor changed when it should not have.");
				SDL_assert(
					false &&
					"InputReplay validation failed, check output log for details or break for debugger");
			}

			if (mBluePortalInfo.mExists)
			{
				if (!mBluePortal)
				{
					SDL_LogWarn(
						0, "Blue portal mismatch.\nExpected: Exists\nActual:   Does not exist");
					SDL_assert(
						false &&
						"InputReplay validation failed, check output log for details or break for debugger");
				}
				else
				{
					ValidateVector("Blue portal position", mBluePortalInfo.mPosition,
								   mBluePortal->GetPosition());
					ValidateQuat("Blue portal quat", mBluePortalInfo.mQuat, mBluePortal->GetQuat());

					float width = 0.0f;
					float height = 0.0f;
					float depth = 0.0f;
					CollisionComponent* cc = mBluePortal->GetComponent<CollisionComponent>();
					if (cc)
					{
						width = cc->GetWidth();
						height = cc->GetHeight();
						depth = cc->GetDepth();
					}
					ValidateFloat("Blue portal width", mBluePortalInfo.mWidth, width);
					ValidateFloat("Blue portal height", mBluePortalInfo.mHeight, height);
					ValidateFloat("Blue portal depth", mBluePortalInfo.mDepth, depth);
				}
			}
			else if (mBluePortal != nullptr)
			{
				SDL_LogWarn(0, "Blue portal mismatch.\nExpected: Does not exist\nActual:   Exists");
				SDL_assert(
					false &&
					"InputReplay validation failed, check output log for details or break for debugger");
			}

			if (mOrangePortal != GetOrangePortal())
			{
				SDL_LogWarn(0, "Orange portal actor changed when it should not have.");
				SDL_assert(
					false &&
					"InputReplay validation failed, check output log for details or break for debugger");
			}

			if (mOrangePortalInfo.mExists)
			{
				if (!mOrangePortal)
				{
					SDL_LogWarn(
						0, "Orange portal mismatch.\nExpected: Exists\nActual:   Does not exist");
					SDL_assert(
						false &&
						"InputReplay validation failed, check output log for details or break for debugger");
				}
				else
				{
					ValidateVector("Orange portal position", mOrangePortalInfo.mPosition,
								   mOrangePortal->GetPosition());
					ValidateQuat("Orange portal quat", mOrangePortalInfo.mQuat,
								 mOrangePortal->GetQuat());

					float width = 0.0f;
					float height = 0.0f;
					float depth = 0.0f;
					CollisionComponent* cc = mOrangePortal->GetComponent<CollisionComponent>();
					if (cc)
					{
						width = cc->GetWidth();
						height = cc->GetHeight();
						depth = cc->GetDepth();
					}
					ValidateFloat("Orange portal width", mOrangePortalInfo.mWidth, width);
					ValidateFloat("Orange portal height", mOrangePortalInfo.mHeight, height);
					ValidateFloat("Orange portal depth", mOrangePortalInfo.mDepth, depth);
				}
			}
			else if (mOrangePortal != nullptr)
			{
				SDL_LogWarn(0,
							"Orange portal mismatch.\nExpected: Does not exist\nActual:   Exists");
				SDL_assert(
					false &&
					"InputReplay validation failed, check output log for details or break for debugger");
			}
		}

		if (static_cast<size_t>(mCurrentPlaybackIndex + 1) == mEvents.size())
		{
			mIsInPlayback = false;
		}
	}
}

void InputReplay::FillPortalInfo(class Actor* portal, PortalInfo& outInfo)
{
	outInfo.mUpdated = true;
	outInfo.mExists = portal != nullptr;
	if (portal)
	{
		outInfo.mPosition = portal->GetPosition();
		outInfo.mQuat = portal->GetQuat();
		CollisionComponent* cc = portal->GetComponent<CollisionComponent>();
		if (cc)
		{
			outInfo.mWidth = cc->GetWidth();
			outInfo.mHeight = cc->GetHeight();
			outInfo.mDepth = cc->GetDepth();
		}
	}
}

void InputReplay::WritePortalJSON(const PortalInfo& info,
								  rapidjson::MemoryPoolAllocator<>& allocator,
								  rapidjson::Value& outJSON)
{
	if (!info.mExists)
	{
		outJSON.AddMember("e", false, allocator);
	}
	else
	{
		outJSON.AddMember("e", true, allocator);

		rapidjson::Value portalPos;
		portalPos.SetArray();
		portalPos.PushBack(info.mPosition.x, allocator);
		portalPos.PushBack(info.mPosition.y, allocator);
		portalPos.PushBack(info.mPosition.z, allocator);
		outJSON.AddMember("p", portalPos, allocator);

		rapidjson::Value portalQuat;
		portalQuat.SetArray();
		portalQuat.PushBack(info.mQuat.x, allocator);
		portalQuat.PushBack(info.mQuat.y, allocator);
		portalQuat.PushBack(info.mQuat.z, allocator);
		portalQuat.PushBack(info.mQuat.w, allocator);
		outJSON.AddMember("q", portalQuat, allocator);

		outJSON.AddMember("w", info.mWidth, allocator);
		outJSON.AddMember("h", info.mHeight, allocator);
		outJSON.AddMember("d", info.mDepth, allocator);
	}
}

void InputReplay::ReadPortalJSON(const rapidjson::Value& value, PortalInfo& outInfo)
{
	outInfo.mUpdated = true;
	outInfo.mExists = value["e"].GetBool();
	if (outInfo.mExists)
	{
		outInfo.mPosition.x = value["p"][0].GetFloat();
		outInfo.mPosition.y = value["p"][1].GetFloat();
		outInfo.mPosition.z = value["p"][2].GetFloat();

		outInfo.mQuat.x = value["q"][0].GetFloat();
		outInfo.mQuat.y = value["q"][1].GetFloat();
		outInfo.mQuat.z = value["q"][2].GetFloat();
		outInfo.mQuat.w = value["q"][3].GetFloat();

		outInfo.mWidth = value["w"].GetFloat();
		outInfo.mHeight = value["h"].GetFloat();
		outInfo.mDepth = value["d"].GetFloat();
	}
}

void InputReplay::ResetPlayer()
{
	if (mGame->GetPlayer())
	{
		if (mGame->GetBluePortal())
		{
			mGame->GetBluePortal()->SetState(ActorState::Destroy);
			mGame->SetBluePortal(nullptr);
		}
		if (mGame->GetOrangePortal())
		{
			mGame->GetOrangePortal()->SetState(ActorState::Destroy);
			mGame->SetOrangePortal(nullptr);
		}
		mGame->GetPlayer()->GetPlayerMove()->ChangeState(MoveState::OnGround);
		mGame->GetPlayer()->GetPlayerMove()->GetCrossHair()->SetState(CrosshairState::Default);
		mGame->GetPlayer()->GetPlayerMove()->SetAcceleration(Vector3::Zero);
		mGame->GetPlayer()->GetPlayerMove()->SetVelocity(Vector3::Zero);
		mGame->GetPlayer()->SetPosition(mGame->GetPlayer()->GetInitialPos());
		mGame->GetPlayer()->SetRotation(0.0f);
		mGame->GetPlayer()->GetCameraComponent()->SetPitchAngle(0.0f);
	}
}

const Vector3& InputReplay::GetPlayerPosition() const
{
	if (mGame->GetPlayer())
	{
		return mGame->GetPlayer()->GetPosition();
	}
	return Vector3::Zero;
}

const Vector3& InputReplay::GetPlayerVelocity() const
{
	if (mGame->GetPlayer())
	{
		return mGame->GetPlayer()->GetPlayerMove()->GetVelocity();
	}
	return Vector3::Zero;
}

const Vector3& InputReplay::GetPlayerAcceleration() const
{
	if (mGame->GetPlayer())
	{
		return mGame->GetPlayer()->GetPlayerMove()->GetAcceleration();
	}
	return Vector3::Zero;
}

float InputReplay::GetPlayerYaw() const
{
	if (mGame->GetPlayer())
	{
		return mGame->GetPlayer()->GetRotation();
	}
	return 0.0f;
}

float InputReplay::GetPlayerPitch() const
{
	if (mGame->GetPlayer())
	{
		return mGame->GetPlayer()->GetCameraComponent()->GetPitchAngle();
	}
	return 0.0f;
}

Actor* InputReplay::GetBluePortal()
{
	if (mGame->GetBluePortal())
	{
		return mGame->GetBluePortal();
	}
	return nullptr;
}

Actor* InputReplay::GetOrangePortal()
{
	if (mGame->GetOrangePortal())
	{
		return mGame->GetOrangePortal();
	}
	return nullptr;
}

#include "PlayerUI.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"

PlayerUI::PlayerUI(Actor* owner)
: UIComponent(owner)
{
	Renderer* r = owner->GetGame()->GetRenderer();
	mLapTextures = std::vector<Texture*>{
		r->GetTexture("Assets/UI/Lap1.png"),
		r->GetTexture("Assets/UI/FinalLap.png"),
	};

	mFirstTexture = r->GetTexture("Assets/UI/1st.png");
	mSecondTexture = r->GetTexture("Assets/UI/2nd.png");

	mGoTexture = r->GetTexture("Assets/UI/Go.png");
	mReadyTexture = r->GetTexture("Assets/UI/Ready.png");
}

void PlayerUI::Update(float deltaTime)
{
	mGoDisplayTimer -= deltaTime;
	mLapDisplayTimer -= deltaTime;
}

void PlayerUI::Draw(Shader* shader)
{
	if (mGoDisplayTimer > 0.0f)
	{
		if (mOwner->GetState() == ActorState::Paused)
		{
			DrawTexture(shader, mReadyTexture, Vector2(0.0f, 100.0f));
		}
		else if (mOwner->GetState() == ActorState::Active)
		{
			DrawTexture(shader, mGoTexture, Vector2(0.0f, 100.0f));
		}
	}

	if (mLapDisplayTimer > 0.0f)
	{
		DrawTexture(shader, mLapTextures[mLapIndex], Vector2(0.0f, 200.0f), 0.75f);
	}

	if (mRaceState == Won)
	{
		DrawTexture(shader, mFirstTexture, Vector2(0.0f, 100.0f));
	}
	else if (mRaceState == Lost)
	{
		DrawTexture(shader, mSecondTexture, Vector2(0.0f, 100.0f));
	}

	// Figure out what place to show
	if (mOwner->GetState() == ActorState::Active)
	{
		bool inFirst = IsPlayerInFirst();
		if (inFirst)
		{
			DrawTexture(shader, mFirstTexture, Vector2(400.0f, 320.0f), 0.5f);
		}
		else
		{
			DrawTexture(shader, mSecondTexture, Vector2(400.0f, 320.0f), 0.5f);
		}
	}
}

void PlayerUI::OnLapChange(int lapNum)
{
	mLapIndex = lapNum - 1;
	mLapDisplayTimer = 3.0f;
	int mPlayerLap = GetGame()->GetPlayer()->GetPlayerMove()->GetLap();
	int mEnemyLap = GetGame()->GetEnemy()->GetEnemyMove()->GetLap();
	if (lapNum == 3)
	{
		mLapIndex = 1;
		GetGame()->GetAudio()->StopSound(GetGame()->GetSoundHandle(), 250);
		if (mPlayerLap > mEnemyLap)
		{
			SetRaceState(PlayerUI::Won);
			GetGame()->GetAudio()->PlaySound("Won.wav");
			GetGame()->GetPlayer()->SetState(ActorState::Paused);
			GetGame()->GetEnemy()->SetState(ActorState::Paused);
		}
		else
		{
			SetRaceState(PlayerUI::Lost);
			GetGame()->GetAudio()->PlaySound("Lost.wav");
			GetGame()->GetPlayer()->SetState(ActorState::Paused);
			GetGame()->GetEnemy()->SetState(ActorState::Paused);
		}
	}
}

bool PlayerUI::IsPlayerInFirst() const
{
	int mPlayerLap = GetGame()->GetPlayer()->GetPlayerMove()->GetLap();
	int mEnemyLap = GetGame()->GetEnemy()->GetEnemyMove()->GetLap();
	int mPlayerLastCheckPoint = static_cast<int>(
		GetGame()->GetPlayer()->GetPlayerMove()->GetCheckPoint() +
		(mPlayerLap - 1) * GetGame()->GetPlayer()->GetPlayerMove()->GetAllCheckPoints().size());
	int mEnemyLastCheckPoint = static_cast<int>(
		GetGame()->GetEnemy()->GetEnemyMove()->GetCheckPoint() +
		(mEnemyLap - 1) * GetGame()->GetEnemy()->GetEnemyMove()->GetAllCheckPoints().size());
	float mPlayerToNext = GetGame()->GetPlayer()->GetPlayerMove()->GetDistanceToNext();
	float mEnemyToNext = GetGame()->GetEnemy()->GetEnemyMove()->GetDistanceToNext();
	if (mPlayerLap > mEnemyLap)
	{
		return true;
	}
	else if (mPlayerLap == mEnemyLap && mPlayerLastCheckPoint > mEnemyLastCheckPoint)
	{
		return true;
	}
	else if (mPlayerLap == mEnemyLap && mPlayerLastCheckPoint == mEnemyLastCheckPoint &&
			 mPlayerToNext < mEnemyToNext)
	{
		return true;
	}
	return false;
}

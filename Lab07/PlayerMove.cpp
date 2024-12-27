#include "PlayerMove.h"
#include "Game.h"
#include "Math.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Player.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "SideBlock.h"
#include "Random.h"
#include "Bullet.h"
#include "Block.h"
class Block;
class Bullet;
class Player;
class SideBlock;
class Renderer;
class MoveComponent;
class Actor;
class Game;
class CollisionComponent;
PlayerMove::PlayerMove(Actor* actor, CollisionComponent* colllisoncomponent)
: MoveComponent(actor)
{
	mCollisionComponent = colllisoncomponent;
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Update(float deltaTime)
{
	mReminderTimer -= deltaTime;
	if (mReminderTimer <= 0.0f && mShield < 3)
	{
		GetGame()->GetPlayer()->GetHUD()->DoABarrelRoll();
		mReminderTimer = Random::GetFloatRange(15.0f, 25.0f);
	}
	if (mIsRolling)
	{
		mRollTimer += deltaTime;
		if (mRollTimer >= ROLLTIME)
		{
			mIsRolling = false;
			mRollTimer = 0.0f;
			GetOwner()->SetRollAngle(0.0f);
		}
		else
		{
			BarrelRoll(mRollTimer);
		}
	}
	Vector3 newPos = GetOwner()->GetPosition() + deltaTime * VELOCITY_AUTO_X * mMultiplier +
					 deltaTime * mMovement * mMultiplier;
	newPos.y = Math::Clamp(newPos.y, -MAX_Y, MAX_Y);
	newPos.z = Math::Clamp(newPos.z, -MAX_Z, MAX_Z);
	GetOwner()->SetPosition(newPos);
	mCntTwo = mCntOne;
	mCntBlockTwo = mCntBlockOne;
	mCntOne = static_cast<int>(GetOwner()->GetPosition().x / STEPSIZE);
	mCntBlockOne = static_cast<int>(GetOwner()->GetPosition().x / (2 * STEPSIZE));
	if (mCntOne != mCntTwo)
	{
		SpawnBlocks(mSideBlockPos + mCntOne * STEPSIZE);
		mSideBlockRound++;
		if (mSideBlockRound == 3)
		{
			mSideBlockRound = 0;
		}
	}
	if (mCntBlockOne != mCntBlockTwo)
	{
		if (mBlockLevel <= 20)
		{
			GetGame()->LoadBlocks("Assets/Blocks/" + std::to_string(mBlockLevel) + ".txt");
		}
		else
		{
			int fileNumber = Random::GetIntRange(1, 20);
			GetGame()->LoadBlocksRandom("Assets/Blocks/" + std::to_string(fileNumber) + ".txt",
										mBlockLevel);
		}
		mBlockLevel++;
	}
	Vector3 eyePos = Vector3(newPos.x - HDIST, newPos.y, 0.0f);
	Vector3 targetPos = Vector3(newPos.x + TARGETDIST, newPos.y, newPos.z);
	Matrix4 viewMatrix = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
	GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
	std::vector<Block*> mBlocks;
	mShieldTimer += deltaTime;
	for (Block* b : GetGame()->GetBlocks())
	{
		if (GetCollisionComponent()->Intersect(b->GetCollisionComponent()))
		{
			if (!b->IsExplode())
			{
				b->SetState(ActorState::Destroy);
			}
			if (b->IsExplode())
			{
				Explode(b->GetPosition(), b, mBlocks);
				for (Block* bb : mBlocks)
				{
					bb->SetState(ActorState::Destroy);
				}
				GetGame()->GetAudio()->PlaySound("BlockExplode.wav");
			}
			if (mShieldTimer >= 1.0f)
			{
				mShield--;
				GetGame()->GetAudio()->PlaySound("ShipHit.wav");
				mShieldTimer = 0.0f;
			}
		}
	}
	if (mShield == 1 && !mAlertPlaying)
	{
		mAlertPlaying = true;
		mAlertSound = GetGame()->GetAudio()->PlaySound("DamageAlert.ogg", true);
	}
	else if (mShield != 1 && mAlertPlaying)
	{
		GetGame()->GetAudio()->StopSound(mAlertSound);
		mAlertPlaying = false;
	}
	if (mShield == 0)
	{
		GetGame()->GetAudio()->StopSound(mAlertSound);
		GetGame()->GetAudio()->PlaySound("ShipDie.wav");
		GetGame()->GetAudio()->StopSound(GetGame()->GetPlayerSound());
		GetGame()->GetPlayer()->SetState(ActorState::Paused);
	}
	mMultiplierTimer += deltaTime;
	if (mMultiplierTimer >= 10.0f)
	{
		mMultiplierTimer = 0.0f;
		mMultiplier += INCREASE;
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mMovement = Vector3::Zero;
	if (keyState[SDL_SCANCODE_Q] && !mQPressed && !mIsRolling)
	{
		if (mShield < 3)
		{
			mShield++;
		}
		GetGame()->GetAudio()->PlaySound("Boost.wav");
		mIsRolling = true;
	}
	mQPressed = keyState[SDL_SCANCODE_Q];
	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed)
	{
		Bullet* b = new Bullet(GetGame());
		GetGame()->GetAudio()->PlaySound("Shoot.wav");
		b->SetPosition(GetOwner()->GetPosition());
	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
	if (keyState[SDL_SCANCODE_W])
	{
		mMovement += VELOCITY_W;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		mMovement += VELOCITY_S;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		mMovement += VELOCITY_A;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		mMovement += VELOCITY_D;
	}
}

void PlayerMove::SpawnBlocks(float i)
{
	if (mSideBlockRound == 0)
	{
		SideBlock* sbOne = new SideBlock(GetGame(), 0); //right
		sbOne->SetPosition(SBONE + Vector3(i, 0.0f, 0.0f));
		sbOne->SetRotation(Math::Pi);

		SideBlock* sbTwo = new SideBlock(GetGame(), 0); //left
		sbTwo->SetPosition(SBTWO + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbThree = new SideBlock(GetGame(), 5); //bottom
		sbThree->SetPosition(SBTHREE + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbFour = new SideBlock(GetGame(), 6); //up
		sbFour->SetPosition(SBFOUR + Vector3(i, 0.0f, 0.0f));
	}
	if (mSideBlockRound == 1)
	{
		SideBlock* sbOne = new SideBlock(GetGame(), 1); //right
		sbOne->SetPosition(SBONE + Vector3(i, 0.0f, 0.0f));
		sbOne->SetRotation(Math::Pi);

		SideBlock* sbTwo = new SideBlock(GetGame(), 1); //left
		sbTwo->SetPosition(SBTWO + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbThree = new SideBlock(GetGame(), 5); //bottom
		sbThree->SetPosition(SBTHREE + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbFour = new SideBlock(GetGame(), 7); //up
		sbFour->SetPosition(SBFOUR + Vector3(i, 0.0f, 0.0f));
	}
	if (mSideBlockRound == 2)
	{
		SideBlock* sbOne = new SideBlock(GetGame(), 2); //right
		sbOne->SetPosition(SBONE + Vector3(i, 0.0f, 0.0f));
		sbOne->SetRotation(Math::Pi);

		SideBlock* sbTwo = new SideBlock(GetGame(), 2); //left
		sbTwo->SetPosition(SBTWO + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbThree = new SideBlock(GetGame(), 5); //bottom
		sbThree->SetPosition(SBTHREE + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbFour = new SideBlock(GetGame(), 6); //up
		sbFour->SetPosition(SBFOUR + Vector3(i, 0.0f, 0.0f));
	}
	if (mSideBlockRound == 3)
	{
		SideBlock* sbOne = new SideBlock(GetGame(), 0); //right
		sbOne->SetPosition(SBONE + Vector3(i, 0.0f, 0.0f));
		sbOne->SetRotation(Math::Pi);

		SideBlock* sbTwo = new SideBlock(GetGame(), 0); //left
		sbTwo->SetPosition(SBTWO + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbThree = new SideBlock(GetGame(), 5); //bottom
		sbThree->SetPosition(SBTHREE + Vector3(i, 0.0f, 0.0f));

		SideBlock* sbFour = new SideBlock(GetGame(), 7); //up
		sbFour->SetPosition(SBFOUR + Vector3(i, 0.0f, 0.0f));
	}
}

void PlayerMove::Explode(Vector3 x, Block* bb, std::vector<Block*>& mBlocks)
{
	for (Block* b : GetGame()->GetBlocks())
	{
		if (b != bb && std::find(mBlocks.begin(), mBlocks.end(), b) == mBlocks.end())
		{
			float mDist = Vector3::Distance(b->GetPosition(), x);
			if (mDist <= RANGE)
			{
				if (!b->IsExplode())
				{
					mBlocks.push_back(b);
				}
				if (b->IsExplode())
				{
					mBlocks.push_back(b);
					Explode(b->GetPosition(), b, mBlocks);
				}
			}
		}
	}
}
void PlayerMove::BarrelRoll(float deltaTime)
{
	GetOwner()->SetRollAngle(ROLL_SPEED * deltaTime);
}
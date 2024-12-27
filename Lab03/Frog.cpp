#include <string>
#include "Game.h"
#include "Frog.h"
#include "SDL2/SDL.h"
#include "DeadFrog.h"
class WrappingMove;
class Actor;
class Game;
class Frog;
class SpriteComponent;
class Vehicle;
class CollisionComponent;
class Log;
Frog::Frog(Game* game, std::string s, float x, float y, int row)
: Actor(game)
{
	Vector2 pos;
	pos.x = x;
	pos.y = y;
	SetPosition(pos);
	CollisionComponent* mFrogCollision = new CollisionComponent(this);
	mFrogCollision->SetSize(FROG_COLLIDER_SIZE, FROG_COLLIDER_SIZE);
	mCollisionComponent = mFrogCollision;
	mInitialPos = pos;
	SetOffset(Vector2::Zero);
	SpriteComponent* sprite = new SpriteComponent(this);
	sprite->SetTexture(GetGame()->GetTexture(s));
	GetGame()->AddSprite(sprite);
	AddToLastFrame(SDL_SCANCODE_W, false);
	AddToLastFrame(SDL_SCANCODE_A, false);
	AddToLastFrame(SDL_SCANCODE_S, false);
	AddToLastFrame(SDL_SCANCODE_D, false);
}

Frog::~Frog()
{
}
void Frog::OnUpdate(float deltaTime)
{
	std::vector<class Vehicle*> mVehiclesCopy = GetGame()->GetVehicles();
	for (Vehicle* v : mVehiclesCopy)
	{
		if (GetCollisionComponent()->Intersect(v->GetCollisionComponent()))
		{
			DeadFrog* mDeadFrog = new DeadFrog(GetGame());
			mDeadFrog->SetPosition(GetPosition());
			SetPosition(mInitialPos);
			SetPosition(mInitialPos);
		}
	}
	for (Log* l : GetGame()->GetLogs())
	{
		CollSide collSide = GetCollisionComponent()->GetMinOverlap(l->GetCollisionComponent(),
																   mOffset);

		if (collSide != CollSide::None)
		{
			mOnLog = true;
			if (!l->GetIsRide())
			{
				l->SetIsRide(true);
				if (collSide == CollSide::Left)
				{
					SetPosition(Vector2(GetPosition().x + mOffset.x + 16.0f, l->GetPosition().y));
				}
				else if (collSide == CollSide::Right)
				{
					SetPosition(Vector2(GetPosition().x + mOffset.x - 16.0f, l->GetPosition().y));
				}
			}
			SetPosition(Vector2(GetPosition().x, l->GetPosition().y));
			SetPosition(Vector2(l->GetWrappingMove()->GetDirection().x *
										(l->GetWrappingMove()->GetForwardSpeed() + SPEED_BALANCE) *
										(l->GetWrappingMove()->GetDeltatime()) +
									GetPosition().x,
								l->GetPosition().y));
			Vector2 pos = GetPosition();

			if (pos.x < 0.0f)
			{
				pos.x = 0.0f;
			}
			else if (pos.x > WINDOW_WIDTH)
			{
				pos.x = WINDOW_WIDTH;
			}

			if (pos.y < 0.0f)
			{
				pos.y = 0.0f;
			}
			else if (pos.y > WINDOW_HEIGHT)
			{
				pos.y = WINDOW_HEIGHT;
			}

			SetPosition(pos);
		}
		else
		{
			l->SetIsRide(false);
		}
	}
	if (GetPosition().y <= Y_UPPER_BOUND && GetPosition().y >= Y_LOWER_BOUND)
	{
		if (!mOnLog)
		{
			DeadFrog* mDeadFrog = new DeadFrog(GetGame());
			mDeadFrog->SetPosition(GetPosition());
			SetPosition(mInitialPos);
			SetPosition(mInitialPos);
		}
	}
	mOnLog = false;
	CheckGoal();
}
void Frog::CheckGoal()
{
	if (GetGame()->GetGoal()->Intersect(GetCollisionComponent()))
	{
		SetPosition(Vector2(GOAL_X, GOAL_Y));
		SetState(ActorState::Paused);
	}
	else if (GetPosition().y <= GOAL_Y)
	{
		DeadFrog* mDeadFrog = new DeadFrog(GetGame());
		mDeadFrog->SetPosition(GetPosition());
		SetPosition(mInitialPos);
		SetPosition(mInitialPos);
	}
}
void Frog::OnProcessInput(const Uint8* keyState)
{
	Vector2 newPos = GetPosition();
	if (keyState[SDL_SCANCODE_W] && !mLastFrame[SDL_SCANCODE_W])
	{
		newPos.y -= SQUARE_LENGTH;
		if (TestInRange(newPos))
		{
			SetPosition(newPos);
		}
		mLastFrame[SDL_SCANCODE_W] = true;
	}
	else if (keyState[SDL_SCANCODE_S] && !mLastFrame[SDL_SCANCODE_S])
	{
		newPos.y += SQUARE_LENGTH;
		if (TestInRange(newPos))
		{
			SetPosition(newPos);
		}
		mLastFrame[SDL_SCANCODE_S] = true;
	}
	else if (keyState[SDL_SCANCODE_A] && !mLastFrame[SDL_SCANCODE_A])
	{
		newPos.x -= SQUARE_LENGTH;
		if (TestInRange(newPos))
		{
			SetPosition(newPos);
		}
		mLastFrame[SDL_SCANCODE_A] = true;
	}
	else if (keyState[SDL_SCANCODE_D] && !mLastFrame[SDL_SCANCODE_D])
	{
		newPos.x += 32;
		if (TestInRange(newPos))
		{
			SetPosition(newPos);
		}
		mLastFrame[SDL_SCANCODE_D] = true;
	}
	if (!keyState[SDL_SCANCODE_W])
	{
		mLastFrame[SDL_SCANCODE_W] = false;
	}
	if (!keyState[SDL_SCANCODE_A])
	{
		mLastFrame[SDL_SCANCODE_A] = false;
	}
	if (!keyState[SDL_SCANCODE_S])
	{
		mLastFrame[SDL_SCANCODE_S] = false;
	}
	if (!keyState[SDL_SCANCODE_D])
	{
		mLastFrame[SDL_SCANCODE_D] = false;
	}
}
bool Frog::TestInRange(Vector2 newPos) const
{
	if (newPos.x >= 0 && newPos.x <= WINDOW_WIDTH)
	{
		if (newPos.y >= 0 && newPos.y <= WINDOW_HEIGHT - SQUARE_LENGTH)
		{
			return true;
		}
	}
	return false;
}
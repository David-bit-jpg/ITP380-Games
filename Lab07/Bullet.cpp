#include "Actor.h"
#include "SDL2/SDL.h"
#include "Bullet.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include <algorithm>
class CollisionComponent;
class MeshComponent;
class MoveComponent;
class Game;
class Renderer;
class Block;
Bullet::Bullet(Game* game)
: Actor(game)
{
	SetScale(5.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CCX, CCY, CCZ);
	MoveComponent* mvc = new MoveComponent(this);
	mvc->SetForwardSpeed(FORWARD_SPEED);
	mMoveComponent = mvc;
	mCollisionComponent = cc;
}

Bullet::~Bullet()
{
}

void Bullet::OnUpdate(float deltaTime)
{
	mTimer += deltaTime;
	std::vector<Block*> mBlocks;
	for (Block* b : GetGame()->GetBlocks())
	{
		if (GetCollisionComponent()->Intersect(b->GetCollisionComponent()))
		{
			if (!b->IsExplode())
			{
				SetState(ActorState::Destroy);
			}
			if (b->IsExplode())
			{
				Explode(b->GetPosition(), b, mBlocks);
				for (Block* bb : mBlocks)
				{
					bb->SetState(ActorState::Destroy);
				}
				SetState(ActorState::Destroy);
			}
		}
	}
	if (mTimer >= 1.0f)
	{
		SetState(ActorState::Destroy);
	}
}
void Bullet::Explode(Vector3 x, Block* bb, std::vector<Block*>& mBlocks)
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

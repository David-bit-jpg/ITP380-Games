#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
: Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
	mPrevDirection = GetDirection();
}

void GhostAI::Update(float deltaTime)
{
	SetInStateTime(deltaTime + GetInStateTime()); //add time in each state
	float mSpeed = 0.0f;
	if (GetState() == State::Scatter || GetState() == State::Chase) //new speed based on state
	{
		mSpeed = SCATTER_CHASE_SPEED;
	}
	else if (GetState() == State::Frightened)
	{
		mSpeed = FRIGHTENED_SPEED;
	}
	else if (GetState() == State::Dead)
	{
		mSpeed = DEAD_SPEED;
	}
	AnimatedSprite* mAnSprite = GetGhost()->GetAnimatedSprite();
	if ((mPrevDirection.x != GetDirection().x || mPrevDirection.y != GetDirection().y) &&
		GetState() != State::Frightened)
	{
		if (GetState() != State::Dead)
		{
			if (GetDirection().x == 1)
			{
				mAnSprite->SetAnimation("right");
			}
			if (GetDirection().x == -1)
			{
				mAnSprite->SetAnimation("left");
			}
			if (GetDirection().y == 1)
			{
				mAnSprite->SetAnimation("down");
			}
			if (GetDirection().y == -1)
			{
				mAnSprite->SetAnimation("up");
			}
		}
		else if (GetState() == State::Dead)
		{
			if (GetDirection().x == 1)
			{
				mAnSprite->SetAnimation("deadright");
			}
			if (GetDirection().x == -1)
			{
				mAnSprite->SetAnimation("deadleft");
			}
			if (GetDirection().y == 1)
			{
				mAnSprite->SetAnimation("deaddown");
			}
			if (GetDirection().y == -1)
			{
				mAnSprite->SetAnimation("deadup");
			}
		}
	}
	if (GetState() == State::Frightened)
	{
		float stateTime = GetInStateTime();
		if (stateTime <= ANIMATION_CHANGE)
		{
			if (mAnSprite->GetAnimName() != "scared0")
			{
				mAnSprite->SetAnimation("scared0");
			}
		}
		else
		{
			if (mAnSprite->GetAnimName() != "scared1")
			{
				mAnSprite->SetAnimation("scared1");
			}
		}
	}

	GetOwner()->SetPosition(GetOwner()->GetPosition() +
							GetDirection() * mSpeed * deltaTime); //update position
	if (GetGhost()->GetCollisionComponent()->Intersect(
			GetNextNode()->GetCollisionComponent())) //if collide with some node
	{
		GetOwner()->SetPosition(GetNextNode()->GetPosition()); //set to that node

		IsChangeState(); //change state is needed
		if (GetState() == State::Scatter)
		{
			SetTargetNode(GetGhost()->GetScatterNode());
		}
		else if (GetState() == State::Frightened)
		{
			SetTargetNode(GetRandomNode(GetNextNode()));
		}
		else if (GetState() == State::Dead)
		{
			SetTargetNode(GetGame()->GetGhostPen());
		}
		else if (GetState() == State::Chase)
		{
			SetTargetNode(GetChaseNode());
		}
		PathNode* mNewNext = GetClosestNode(GetNextNode()); //get closest node with 3 restrictions
		if (mNewNext == nullptr)
		{
			mNewNext = GetClosestNodeGhost(GetNextNode()); //if not, include ghost node
		}
		if (mNewNext == nullptr)
		{
			mNewNext = GetClosestNodeAny(GetNextNode()); //if also not, pick any kind of nodes
		}
		SetPrevNode(GetNextNode());
		SetNextNode(mNewNext);
		mPrevDirection = GetDirection();
		CalculateDirection(); //recalculate direction
	}
}

void GhostAI::Frighten()
{
	if (GetState() == State::Dead)
	{
		return;
	}

	SetInStateTime(0.0f);
	if (GetState() != State::Frightened)
	{
		SetState(State::Frightened);
		std::swap(mPrevNode, mNextNode);
		CalculateDirection();
		SetTargetNode(GetRandomNode(GetNextNode()));
	}
}

void GhostAI::Start(PathNode* startNode)
{
	SetInStateTime(0.0f);
	GetOwner()->SetPosition(startNode->GetPosition());
	SetState(State::Scatter);
	SetPrevNode(nullptr);
	SetNextNode(startNode);
}

PathNode* GhostAI::GetChaseNode()
{
	PathNode* targetNode = nullptr;
	if (GetGhost()->GetType() == Ghost::Type::Blinky)
	{
		targetNode = GetGame()->GetPlayer()->GetPrevNode();
		if (targetNode->GetType() == PathNode::Type::Tunnel) //if it's a tunnel node
		{
			targetNode = GetClosestNodeDefault(targetNode); //pick nearest default
		}
	}
	if (GetGhost()->GetType() == Ghost::Type::Pinky)
	{
		Vector2 newPos = GetGame()->GetPlayer()->GetPointInFrontOf(POINT_IN_FRONT_CHASE);
		targetNode = GetClosestNodeByPos(newPos);
	}
	if (GetGhost()->GetType() == Ghost::Type::Inky)
	{
		Vector2 mP = GetGame()->GetPlayer()->GetPointInFrontOf(POINT_IN_FRONT_CHASE / 2);
		std::array<class Ghost*, GHOST_COUNT> mGhostsCpy = GetGame()->GetGhosts();
		Vector2 mBlinkyPos = mGhostsCpy[0]->GetPosition(); //get blinky pos
		Vector2 newPos = mP - mBlinkyPos;
		newPos *= 2;
		Vector2 mQ = newPos + mBlinkyPos;
		targetNode = GetClosestNodeByPos(mQ);
	}
	if (GetGhost()->GetType() == Ghost::Type::Clyde)
	{
		float mDis =
			Vector2::Distance(GetGame()->GetPlayer()->GetPosition(),
							  GetGhost()->GetPosition()); //get distance between player and clyde
		if (mDis > CLYDE_THRESHOLD)						  //if less than threshold
		{
			targetNode = GetGame()->GetPlayer()->GetPrevNode(); //go to player's prev node
			if (targetNode->GetType() == PathNode::Type::Tunnel)
			{
				targetNode =
					GetClosestNodeDefault(targetNode); //if it's tunnel, get nearest default
			}
		}
		else
		{
			targetNode = GetGhost()->GetScatterNode(); //not in threshold, just go to scatter node
		}
	}
	return targetNode;
}

void GhostAI::Die()
{
	SetInStateTime(0.0f);
	SetState(State::Dead);
	CalculateDirection();
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render, static_cast<int>(mOwner->GetPosition().x),
						   static_cast<int>(mOwner->GetPosition().y),
						   static_cast<int>(mNextNode->GetPosition().x),
						   static_cast<int>(mNextNode->GetPosition().y));
	}
}
PathNode* GhostAI::GetClosestNode(PathNode* node)
{
	std::vector<PathNode*> mAdjacents = node->GetAdjacentNodes(); //get all adjacent nodes
	float mDistance = static_cast<float>(UINT_MAX);
	PathNode* mNext = nullptr;
	for (PathNode* pn : mAdjacents)
	{
		float d = Vector2::Distance(pn->GetPosition(),
									mTargetNode->GetPosition()); //get pos from target to this node
		if (mDistance >= d && pn != GetPrevNode() &&
			pn->GetType() != PathNode::Tunnel) //if not prev not tunnel
		{
			if (GetState() != State::Dead) //not dead, because dead can pick ghost
			{
				if (pn->GetType() != PathNode::Ghost) //also not ghots
				{
					mDistance = d;
					mNext = pn;
				}
			}
			else
			{
				mDistance = d;
				mNext = pn;
			}
		}
	}
	return mNext;
}
PathNode* GhostAI::GetClosestNodeGhost(PathNode* node)
{
	std::vector<PathNode*> mAdjacents = node->GetAdjacentNodes();
	float mDistance = static_cast<float>(UINT_MAX);
	PathNode* mNext = nullptr;
	for (PathNode* pn : mAdjacents)
	{
		float d = Vector2::Distance(pn->GetPosition(), mTargetNode->GetPosition());
		if (mDistance >= d && pn != GetPrevNode() && pn->GetType() != PathNode::Tunnel)
		{
			mDistance = d;
			mNext = pn;
		}
	}
	return mNext;
}
PathNode* GhostAI::GetClosestNodeAny(PathNode* node)
{
	std::vector<PathNode*> mAdjacents = node->GetAdjacentNodes();
	float mDistance = static_cast<float>(UINT_MAX);
	PathNode* mNext = nullptr;
	for (PathNode* pn : mAdjacents)
	{
		float d = Vector2::Distance(pn->GetPosition(), mTargetNode->GetPosition());
		if (mDistance >= d)
		{
			mDistance = d;
			mNext = pn;
		}
	}
	return mNext;
}
PathNode* GhostAI::GetClosestNodeDefault(PathNode* node)
{
	std::vector<PathNode*> mAdjacents = node->GetAdjacentNodes();
	float mDistance = static_cast<float>(UINT_MAX);
	PathNode* mNext = nullptr;
	for (PathNode* pn : mAdjacents)
	{
		float d = Vector2::Distance(pn->GetPosition(), mTargetNode->GetPosition());
		if (mDistance >= d && pn->GetType() == PathNode::Default)
		{
			mDistance = d;
			mNext = pn;
		}
	}
	return mNext;
}
PathNode* GhostAI::GetClosestNodeByPos(Vector2 node)
{
	std::vector<PathNode*> mPathNodesCpy = GetGame()->GetPathNodes();
	float mDistance = static_cast<float>(UINT_MAX);
	PathNode* mNext = nullptr;
	for (PathNode* pn : mPathNodesCpy)
	{
		float d = Vector2::Distance(pn->GetPosition(), node);
		if (mDistance >= d && pn->GetType() == PathNode::Default)
		{
			mDistance = d;
			mNext = pn;
		}
	}
	return mNext;
}
Vector2 GhostAI::CalculateDirection()
{
	Vector2 mD = GetNextNode()->GetPosition() - GetPrevNode()->GetPosition();
	mD.Normalize();
	SetDirection(mD);
	return mD;
}
PathNode* GhostAI::GetRandomNode(PathNode* node)
{
	std::vector<PathNode*> mAdjacents = node->GetAdjacentNodes();
	PathNode* mNext = nullptr;
	std::vector<PathNode*> mThree; //not tunnel, ghost, prev
	std::vector<PathNode*> mTwo;   //not tunnel nodes,prev
	std::vector<PathNode*> mNone;  //for any kinds of nodes
	for (PathNode* pn : mAdjacents)
	{
		mNone.push_back(pn);
		if (pn != GetPrevNode() && pn->GetType() != PathNode::Tunnel)
		{
			if (pn->GetType() != PathNode::Ghost)
			{
				mThree.push_back(pn);
			}
		}
		if (pn != GetPrevNode() && pn->GetType() != PathNode::Tunnel)
		{
			mTwo.push_back(pn);
		}
	}
	if (!mThree.empty())
	{
		int randomIndex =
			static_cast<int>(Random::GetIntRange(0, static_cast<int>(mThree.size()) - 1));
		mNext = mThree[randomIndex];
	}
	else if (!mTwo.empty())
	{
		int randomIndex =
			static_cast<int>(Random::GetIntRange(0, static_cast<int>(mTwo.size()) - 1));
		mNext = mTwo[randomIndex];
	}
	else if (mNone.empty())
	{
		int randomIndex =
			static_cast<int>(Random::GetIntRange(0, static_cast<int>(mNone.size()) - 1));
		mNext = mNone[randomIndex];
	}
	return mNext;
}
void GhostAI::IsChangeState()
{
	if (GetState() == State::Frightened &&
		GetInStateTime() > CHANGE_STATE_FS) //if frightened exceed time
	{
		SetState(State::Scatter); //back to scatter
		SetInStateTime(0.0f);
	}
	else if (GetState() == State::Dead &&
			 GetGhost()->GetCollisionComponent()->Intersect(
				 GetGame()
					 ->GetGhostPen()
					 ->GetCollisionComponent())) //if it's dead and get to ghostpen
	{
		SetState(State::Scatter); //reset to sactter
		SetInStateTime(0.0f);
	}
	else if (GetState() == State::Scatter && GetInStateTime() > CHANGE_STATE_SC)
	{
		SetState(State::Chase);
		SetInStateTime(0.0f);
	}
	else if (GetState() == State::Chase && GetInStateTime() > CHANGE_STATE_CS)
	{
		SetState(State::Scatter);
		SetInStateTime(0.0f);
	}
}

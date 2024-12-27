#include "Actor.h"
#include "SDL2/SDL.h"
#include "Block.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
class CollisionComponent;
class MeshComponent;
class Game;
class Renderer;

Block::Block(Game* game, size_t textureIndex)
: Actor(game)
{
	SetScale(25.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mc->SetTextureIndex(textureIndex);
	mMeshComponent = mc;
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(CCX, CCY, CCZ);
	mCollisionComponent = cc;
	if (textureIndex == 4)
	{
		mIsExploding = true;
	}
}

Block::~Block()
{
	GetGame()->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
	Vector3 mPlayerPos = GetGame()->GetPlayer()->GetPosition();
	float mDist = Vector3::Distance(mPlayerPos, GetPosition());
	if (mDist >= THRESHOLD && GetPosition().x < mPlayerPos.x)
	{
		SetState(ActorState::Destroy);
	}
}

#include "Actor.h"
#include "SDL2/SDL.h"
#include "SideBlock.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
class CollisionComponent;
class MeshComponent;
class Game;

SideBlock::SideBlock(Game* game, size_t textureIndex)
: Actor(game)
{
	SetScale(SCALE);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mc->SetTextureIndex(textureIndex);
	mMeshComponent = mc;
}

SideBlock::~SideBlock()
{
}

void SideBlock::OnUpdate(float deltaTime)
{
	Vector3 mPlayerPos = GetGame()->GetPlayer()->GetPosition();
	float mDist = Vector3::Distance(mPlayerPos, GetPosition());
	if (mDist >= THRESHOLD && GetPosition().x < mPlayerPos.x)
	{
		SetState(ActorState::Destroy);
	}
}

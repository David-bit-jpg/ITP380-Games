#include "Portal.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

Portal::Portal(class Game* game, bool isBlue)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, true);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Portal.gpmesh"));
	PortalMeshComponent* pmc = new PortalMeshComponent(this);
	if (isBlue)
	{
		pmc->SetTextureIndex(0);
		mc->SetTextureIndex(2);
	}
	else
	{
		pmc->SetTextureIndex(1);
		mc->SetTextureIndex(3);
	}
}

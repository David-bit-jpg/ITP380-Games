#include "PathNode.h"
#include "SpriteComponent.h"
#include "Game.h"
#include <string>
#include "CollisionComponent.h"

PathNode::PathNode(class Game* game, Type type)
: Actor(game)
, mType(type)
{
	SpriteComponent* sc = new SpriteComponent(this);

	std::string texName = "Assets/Node.png";
	if (mType == Ghost)
	{
		texName = "Assets/NodeGhost.png";
	}
	else if (mType == Tunnel)
	{
		texName = "Assets/NodeTunnel.png";
	}
	sc->SetTexture(game->GetTexture(texName));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(1.0f, 1.0f);
	SetCollisionComponent(cc);
}

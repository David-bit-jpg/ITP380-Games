#include "Actor.h"
#include "SDL2/SDL.h"
#include "Prop.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
Prop::Prop(Game* game)
: Actor(game)
{
	GetGame()->AddColliders(this);
}

Prop::~Prop()
{
	GetGame()->RemoveColliders(this);
}
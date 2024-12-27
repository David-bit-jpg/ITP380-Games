#include "Effect.h"
#include "Actor.h"
#include "Game.h"
#include <string>
#include "AnimatedSprite.h"
class AnimatedSprite;
Effect::Effect(Game* game, Vector2 pos, std::string animiname, std::string soundname)
: Actor(game)
{
	AnimatedSprite* sprite = new AnimatedSprite(this, 200);
	sprite->GetOwner()->SetPosition(pos);
	mAnimatedSprite = sprite;
	mAnimatedSprite->LoadAnimations("Assets/Effects");
	mAnimatedSprite->SetAnimation(animiname);
	GetGame()->GetAudio()->PlaySound(soundname);
	SetLifeTime(mAnimatedSprite->GetAnimDuration(animiname));
}

Effect::~Effect()
{
}

void Effect::OnUpdate(float deltaTime)
{
	mLifeTime -= deltaTime;
	if (mLifeTime <= 0.0f)
	{
		SetState(ActorState::Destroy);
	}
}

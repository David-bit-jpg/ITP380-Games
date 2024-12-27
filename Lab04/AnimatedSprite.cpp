#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (!mAnimName.empty())
	{
		if (!mIsPaused)
		{
			mAnimTimer += deltaTime;
		}

		auto animIterator = mAnims.find(mAnimName);
		if (animIterator != mAnims.end())
		{
			float totalDuration = animIterator->second.size() / static_cast<float>(mAnimFPS);
			while (mAnimTimer >= totalDuration)
			{
				mAnimTimer -= totalDuration;
			}
			int currentFrame = static_cast<int>(mAnimTimer * mAnimFPS) %
							   animIterator->second.size();
			SetTexture(animIterator->second[currentFrame]);
		}
	}
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}

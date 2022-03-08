#include "AnimationComponent.hpp"

//animationComponent -----------------------

//constructor / destructor

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& tex) :
	sprite(sprite),
	textureSheet(tex),
	priorityAnim(NULL),
	endedPriorityAnim(false),
	spriteScale(sprite.getScale()) //gets initial scale of sprite
{
}

AnimationComponent::~AnimationComponent()
{
	for (auto a : anims)
	{
		delete a.second;
	}
}

void AnimationComponent::CreateAnimation(const std::string& animName, sf::Sprite& sprite, sf::Texture& tex,
	int width, int height, int startX, int Yposition,
	int frames, float frameTime)
{
	this->anims[animName] = new Animation(sprite, tex, width, height, startX, Yposition, frames, frameTime);
}

//functions
bool AnimationComponent::Play(const std::string& animName, float dt, bool isPriority, float speedModPercent, bool reset)
{
	if (isPriority == true && priorityAnim != anims.at(animName))
	{									   // if is priority and hasn't been assigned,
		priorityAnim = anims.at(animName); //assing it as as priority
	}

	if (priorityAnim != NULL)
	{ //if there is priority play it c instead of the normal one animation
		if (priorityAnim->IsCycleDone() == true)
		{
			priorityAnim->cycleDone = false;
			priorityAnim = NULL;
			endedPriorityAnim = true;
			return true;
		}
		else
		{
			endedPriorityAnim = false;
			priorityAnim->Play(dt);
		}
	}
	else
	{
		endedPriorityAnim = false;
		anims.at(animName)->Play(dt, speedModPercent, reset);
	}

	return false;
}

bool AnimationComponent::GetEndedPriorityAnim()
{
	return endedPriorityAnim;
}

bool AnimationComponent::GetIsFlipped()
{
	return isFlipped;
}

void AnimationComponent::Flip(bool dir)
{
	if (dir == false)
	{ //if dir left try to flip
		if (!isFlipped)
		{ // if hasn't been flipped yet, flip it
			this->sprite.setScale(-spriteScale.x, spriteScale.y);
			this->isFlipped = true;
		}
	}
	else
	{ // if right, reset flip if is flipped
		if (isFlipped)
		{
			this->sprite.setScale(spriteScale.x, spriteScale.y);
			this->isFlipped = false;
		}
	}
}

bool AnimationComponent::EndedCycle(const std::string& animName)
{
	if (anims.at(animName)->cycleDone)
	{
		return true;
	}
	return false;
}

void AnimationComponent::StopAnimationAt(int x, int y, int width, int height)
{
	sf::IntRect tempRect = sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(width, height));
	sprite.setTextureRect(tempRect);
}

//animation ----------------------

//constructor / destructor
Animation::Animation(sf::Sprite& sprite, sf::Texture& tex,
	int width, int height, int startX, int Yposition,
	int frames, float frameTime) :
	sprite(sprite),
	textureSheet(tex)
{
	startRect.width = width;			//
	startRect.height = height;			//		start rect basic values
	startRect.left = startX * width;	//
	startRect.top = Yposition * height; //

	currentRect = startRect; //sets current rect to start rect so it can start from it

	endRect.width = width;
	endRect.height = height;
	endRect.left = frames * width;	  //calcs the X end
	endRect.top = Yposition * height; //calcs the X end

	this->timer = 0.f; //inits timer
	this->animTime = frameTime;
	this->cycleDone = false;

	this->sprite.setTextureRect(startRect);
}

Animation::~Animation()
{
}

void Animation::Play(float dt, float speedModPercent, bool reset)
{
	cycleDone = false;

	if (speedModPercent < 0.5f)
	{
		speedModPercent = 0.5f;
	}
	this->timer += speedModPercent * dt;
	if (timer > animTime)
	{
		timer = 0.f; // resets timer

		if (currentRect != endRect)
		{
			currentRect.left += currentRect.width;
		}
		else //done
		{
			if (reset == true)
			{
				currentRect = startRect; //cycledone todou
			}
			cycleDone = true;
		}

		sprite.setTextureRect(currentRect);
	}
}

bool Animation::IsCycleDone()
{
	return cycleDone;
}

int Animation::GetFrames()
{
	return endRect.left / 32;
}

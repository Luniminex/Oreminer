#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <map>

//class that manages single animation(walk/idle/attack..)

class Animation
{
	//variables
public:
	sf::Sprite& sprite; //points to entity sprite
	sf::Texture& textureSheet;

	sf::IntRect startRect;	 //default start point of anim
	sf::IntRect currentRect; //current position of anim
	sf::IntRect endRect;	 //default end point of anim

	float animTime; //time that one frame of animation takes
	float timer;
	bool cycleDone;

	//functions
public:
	Animation(sf::Sprite& sprite, sf::Texture& tex,
		int width, int height, int startX, int Yposition,
		int frames, float frameTime);

	~Animation();

	//plays animation
	void Play(float dt, float speedModPercent = 1.f, bool reset = true);

	int GetFrames();

	bool IsCycleDone();
};

class AnimationComponent
{ // ---!--!!--!-!-!-!-!-!- priority anim funguji divne

	//variables
private:
	//stores all the avaiable animations
	std::map<std::string, Animation*> anims;
	sf::Sprite& sprite; //points to entity sprite
	sf::Texture& textureSheet;

	Animation* priorityAnim;

	bool endedPriorityAnim;
	bool isFlipped;
	sf::Vector2f spriteScale;

	//functions
public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& tex);

	~AnimationComponent();

	//creates an animation from texture sheet
	void CreateAnimation(const std::string& animName, sf::Sprite& sprite, sf::Texture& tex,
		int width, int height, int startX, int startY,
		int frames, float frameTime);

	//plays certain animation, returns true if priority anim ended cycle
	bool Play(const std::string& animName, float dt, bool isPriority, float speedModPercent = 100.f, bool reset = true);

	//returns true if priority animations ended
	bool GetEndedPriorityAnim();

	//returns true if cycle of certain anim ended
	bool EndedCycle(const std::string& animName);

	//returns true if sprite is flipped
	bool GetIsFlipped();

	//flips the sprite
	//:false - left
	//:true - right
	void Flip(bool toFlip);

	void StopAnimationAt(int x, int y, int width, int height);
};

#endif
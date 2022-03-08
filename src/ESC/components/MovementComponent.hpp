#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "observers/AudioPlayer.hpp"

enum MVMNT_STATES
{
	IDLE = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//enables position and movement
class MovementComponent
{
	//variables
public:
	float posx;		   //x position in the world
	float posy;		   //y position in the world
	unsigned int posz; //z position in the world - layer 0 to x
	//maximum velocity able to achieve
	float maxVelocity; //maximum velocity entity can achieve

	sf::Vector2f velocity; //current velocity of entity

	float acceleration; //acceleration per update
	float deceleration; //decceleration per update

	//constructors
public:
	MovementComponent(float maxVelocity, float acceleration,
		float decelerationfloat, float x = 0.f, float y = 0.f, unsigned int z = 0.f);

	//accessors
public:
	//rerutrns max velocity
	float GetMaxVelocity();

	//returns current velocity
	sf::Vector2f GetVelocity();

	//returns booleand dependant on which state you send in from movement states
	bool GetState(const short unsigned state);

	//if not moving, idle -> true
	bool IsIdle();

	//returns true if is moving left
	bool IsMovingLeft();

	//returns true if is moving right
	bool IsMovingRight();

	//returns true if is moving up
	bool IsMovingUp();

	//returns true if is moving down
	bool IsMovingDown();

	unsigned int GetZ();

	//modifiers
public:
	//sets velocity to 0
	void ResetVelocity();

	//functions
public:
	//sets the velocity
	void Move(float dirX, float dirY);

	//moves player
	void Update(const float& dt);
};
#endif

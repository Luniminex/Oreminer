#include "ESC/components/MovementComponent.hpp"

//constructor / destructor

MovementComponent::MovementComponent(float maxVelocity, float acceleration,
	float deceleration, float x, float y, unsigned int z)
{
	posx = x;
	posy = y;
	posz = z;
	this->maxVelocity = maxVelocity;
	this->acceleration = acceleration;
	this->deceleration = deceleration;
}

//public functions
void MovementComponent::Move(float dirX, float dirY)
{
	//x
	velocity.x = (dirX * acceleration) + velocity.x;
	if (velocity.x > maxVelocity) //max velocity check right
	{
		velocity.x = maxVelocity;
	}
	else if (velocity.x < -maxVelocity) //max velocity check left
	{
		velocity.x = -maxVelocity;
	}
	//y
	velocity.y = (dirY * acceleration) + velocity.y;
	if (velocity.y > maxVelocity) //max velocity check down
	{
		velocity.y = maxVelocity;
	}
	else if (velocity.y < -maxVelocity) //max velocity check up
	{
		velocity.y = -maxVelocity;
	}
}

void MovementComponent::Update(const float& dt)
{
	//deceleration check
	if (velocity.x > 0)
	{
		velocity.x = velocity.x - deceleration;
		if (velocity.x < 0)
		{
			velocity.x = 0;
		}
	}
	if (velocity.x < 0)
	{
		velocity.x = velocity.x + deceleration;
		if (velocity.x > 0)
		{
			velocity.x = 0;
		}
	}
	if (velocity.y > 0)
	{
		velocity.y = velocity.y - deceleration;
		if (velocity.y < 0)
		{
			velocity.y = 0;
		}
	}
	if (velocity.y < 0)
	{
		velocity.y = velocity.y + deceleration;
		if (velocity.y > 0)
		{
			velocity.y = 0;
		}
	}

	//position set
	posx = posx + (velocity.x * dt);
	posy = posy + (velocity.y * dt);

	if (velocity.x != 0 || velocity.y != 0)
	{
		AudioPlayer::PlaySoundOnLoop("WALKING", true);
	}
	else
	{
		AudioPlayer::PlaySoundOnLoop("WALKING", false);
	}
}

//accesors
float MovementComponent::GetMaxVelocity()
{
	return maxVelocity;
}

sf::Vector2f MovementComponent::GetVelocity()
{
	return velocity;
}

bool MovementComponent::GetState(const short unsigned state)
{
	/*(void)state;
	return true;*/

	switch (state)
	{
		case 0:
			return IsIdle();
			break;
		case 1:
			return IsMovingLeft();
			break;
		case 2:
			return IsMovingRight();
			break;
		case 3:
			return IsMovingUp();
			break;
		case 4:
			return IsMovingDown();
			break;
		default:
			return IsIdle();
	}
}

bool MovementComponent::IsIdle()
{
	if (velocity.x == 0.f && velocity.y == 0.f)
	{
		return true;
	}
	return false;
}

bool MovementComponent::IsMovingLeft()
{
	if (velocity.x < 0.f)
	{
		return true;
	}
	return false;
}

bool MovementComponent::IsMovingRight()
{
	if (velocity.x > 0.f)
	{
		return true;
	}
	return false;
}

bool MovementComponent::IsMovingUp()
{
	if (velocity.y < 0.f)
	{
		return true;
	}
	return false;
}

bool MovementComponent::IsMovingDown()
{
	if (velocity.y > 0.f)
	{
		return true;
	}
	return false;
}

unsigned int MovementComponent::GetZ()
{
	return this->posz;
}

void MovementComponent::ResetVelocity()
{
	velocity = { 0.f, 0.f };
}

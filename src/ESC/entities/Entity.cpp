#include "Entity.hpp"
//constructor / desctructor

Entity::Entity()
{
	this->keyTime = 0.f;
	this->keyTimeMax = 15.f;
}

Entity::~Entity()
{

}

bool Entity::GetKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

void Entity::UpdateKeyTime(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

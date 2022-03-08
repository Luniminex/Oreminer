#include "ESC/components/MiningComponent.hpp"

//constructor / destructor

MiningComponent::MiningComponent(sf::Vector2f offset)
{
	this->offset = offset;
	this->mining = false;
	this->mined = false;
}

MiningComponent::~MiningComponent()
{
}

//public functions

sf::Vector2f MiningComponent::MineRight(sf::FloatRect hitboxRect)
{
	return sf::Vector2f(hitboxRect.left + hitboxRect.width + offset.x, hitboxRect.top + (hitboxRect.height / 2.f) + offset.y);
}

sf::Vector2f MiningComponent::MineLeft(sf::FloatRect hitboxRect)
{
	return sf::Vector2f(hitboxRect.left - offset.x, hitboxRect.top + (hitboxRect.height / 2.f) + offset.y);
}

#include "ESC/components/HitboxComponent.hpp"

//constructors / destructors

HitboxComponent::HitboxComponent(sf::Sprite& sprite, sf::Vector2f size)
{
	this->size = size;
	box.setSize(size);
	box.setPosition(sf::Vector2f(sprite.getPosition().x - (size.x / 2.f), sprite.getPosition().y));
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color::Red);
	box.setOutlineThickness(2.f);

	nextPos.height = size.y;
	nextPos.width = size.x;
	nextPos.left = 0.f;
	nextPos.top = 0.f;
}

HitboxComponent::~HitboxComponent()
{
}

sf::FloatRect HitboxComponent::GetGlobalBounds()
{
	return box.getGlobalBounds();
}

sf::Vector2f HitboxComponent::GetSize()
{
	return size;
}

sf::FloatRect& HitboxComponent::GetNextPosition(sf::Vector2f velocity, float dt)
{

	nextPos.left = box.getPosition().x + (velocity.x * dt);
	nextPos.top = box.getPosition().y + (velocity.y * dt);

	return nextPos;
}

sf::Vector2f HitboxComponent::GetPosition()
{
	return box.getPosition();
}

void HitboxComponent::SetPosition(sf::Vector2f pos)
{
	box.setPosition(sf::Vector2f(pos.x - (size.x / 2.f), pos.y));
}

bool HitboxComponent::Collided(sf::FloatRect rect, sf::Vector2f velocity, float dt)
{
	if (GetNextPosition(velocity, dt).intersects(rect))
	{
		return true;
	}
	return false;
}

int HitboxComponent::CollidedRight(sf::FloatRect rect)
{
	boxPos = box.getPosition() + (box.getSize() / 2.f);
	otherPos = sf::Vector2f(rect.left + (rect.width / 2.f), (rect.top + rect.height / 2.f));
	boxHalfSize = box.getSize() / 2.f;
	otherHalfSize = sf::Vector2f(rect.width / 2.f, rect.height / 2.f);

	deltaX = otherPos.x - boxPos.x; // distance between box and other on X axis
	deltaY = otherPos.y - boxPos.y; // distance between box and other on Y axis

	intersectX = std::abs(deltaX) - (otherHalfSize.x + boxHalfSize.x); // if negative, they are colliding on X axis
	intersectY = std::abs(deltaY) - (otherHalfSize.y + boxHalfSize.y); // if negative, they are colliding on Y axis

	if (intersectX < 0.f && intersectY < 0.f)
	{ //there is collision

		if (intersectX > intersectY)
		{ // bigger collision on X
			if (deltaX > 0.f)
			{ // collision on the left side of box
				return CollisionSide::right;
			}
			else
			{ // collision on the right side of box
				return CollisionSide::left;
			}
		}
		else
		{
			if (deltaY > 0.f)
			{ //collision on bottom of box
				return CollisionSide::down;
			}
			else
			{ // collision on top of box
				return CollisionSide::top;
			}
		}
	}

	return -1; //no collision
}

void HitboxComponent::Update(sf::Vector2f position)
{
	box.setPosition(sf::Vector2f(position.x - (size.x / 2.f), position.y));
}

void HitboxComponent::Display(sf::RenderTarget& trg)
{
	trg.draw(box);
}

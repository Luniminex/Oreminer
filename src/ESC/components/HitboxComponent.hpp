#ifndef HITBOXCOMPONENT_HPP
#define HITBOXCOMPONENT_HPP

#include <SFML/Graphics.hpp>

enum CollisionSide{top = 0, right = 1, down = 2, left = 3};

class HitboxComponent
{
private:
	sf::RectangleShape box; // box around entity that collides
	sf::FloatRect nextPos;
	sf::Vector2f size;

	sf::Vector2f boxPos; //center pos of box
	sf::Vector2f otherPos; //center pos of other
	sf::Vector2f boxHalfSize; //half size of box
	sf::Vector2f otherHalfSize; //half size of other

	float intersectX;
	float intersectY;

	float deltaX;
	float deltaY;


	//constructors
public:

	HitboxComponent(sf::Sprite& sprite, sf::Vector2f size);
	~HitboxComponent();

	//functions
public:

	//returns float rect of hitbox
	sf::FloatRect GetGlobalBounds();

	//returns size of hitbox
	sf::Vector2f GetSize();

	//returns next position of hitbox
	sf::FloatRect& GetNextPosition(sf::Vector2f velocity, float dt);

	//returns position of hitbox
	sf::Vector2f GetPosition();
	//modifiers
public:
	void SetPosition(sf::Vector2f pos);

	//functions
public:
	//checks if there is any collision
	bool Collided(sf::FloatRect rect, sf::Vector2f velocity, float dt);

	//checks for collision and return on what side they collided
	int CollidedRight(sf::FloatRect rect);

	//updates hitbox
	void Update(sf::Vector2f position);

	//displays hitbox
	void Display(sf::RenderTarget& trg);



};

#endif
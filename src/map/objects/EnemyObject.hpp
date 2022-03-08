#ifndef ENEMYOBJECT_HPP
#define ENEMYOBJECT_HPP

#include "map/Tile.hpp"
#include "ESC/components/AnimationComponent.hpp"

class EnemyObject : public Tile
{

	//variables
public:
	int ID;
	Animation anim;
	sf::RectangleShape senseArea; //area the enemy is able to sense in
	bool sensing;
	bool dead;


	//constructor / destructor
public:
	EnemyObject(int ID, bool dead, int gridPosX, int gridPosY, int gridSize, sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int frames);
	~EnemyObject();
	//accessors
public:

	int GetID();

	//returns true if is sensing
	bool IsSensing();

	//returns if true if dead
	bool IsDead();

	//modifiers
public:

	//enables or disables sensing
	void SetSense(bool toSense);

	//set entity dead of alive
	virtual void SetDead(bool isDead);

	//public functions
public:
	//returns true if points is located inside sense area
	bool IsInSenseArea(sf::Vector2f point);

	virtual void Update(float dt);

	virtual void Display(sf::RenderTarget& trg);
};

#endif

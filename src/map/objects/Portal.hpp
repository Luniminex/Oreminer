#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "Object.hpp"

class Tile;
class Object;

class Portal : public Tile
{
public:
private:
	int mapID; //holds ID of map that should be loaded when entered
	bool accessible;

	//constructor / destructor
public:
	Portal(int gridPosX, int gridPosY, int gridSize, const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int mapID, bool accessible);

	~Portal();

	//accessors
public:
	//returns ID of map that should be loaded
	int GetID();

	//returns true if portal is opened
	bool GetAccess();

	//returns true if tile contains point
	bool ContainsPoint(sf::Vector2f point);
	//modifiers
public:
	//sets map ID to load
	void SetMapID(int ID);

	//sets access to enter portal either to true or false
	void SetAccess(bool access);
	//public functions
public:
	//updates tile
	virtual void Update(float dt);

	//renders tile to win
	virtual void Display(sf::RenderTarget& trg);
};

#endif
#ifndef TILE_HPP
#define TILE_HPP

enum tileTypes
{
	tile,
	object,
	portal,
	enemy,
	npc
};

class Tile
{

	//variables
public:
	int type; //type of tile
	sf::Sprite tile;
	bool collision; //creates collider if is true

	//constructors
public:
	Tile(int gridPosX, int gridPosY, int gridSize, const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision);

	Tile();

	virtual ~Tile();

	//accessors
public:
	//returns ID of object
	virtual int GetID();

	//returns count of objects
	virtual unsigned int GetUniqueID();

	//return int texture rect of tile
	sf::IntRect GetRect();

	//returns if is collision enabled
	bool isCollision();

	//returns type of tile
	int GetType();

	//returns position of tile in world
	sf::Vector2f GetPosition();

	//returns global bounds of tile
	sf::FloatRect GetGlobalBounds();

	//modifiers
public:
	//changes texture of tile
	void ChangeTexture(const sf::IntRect& texRect);

	//functions
public:
	//updates tile
	virtual void Update(float dt);

	//renders tile to win
	virtual void Display(sf::RenderTarget& trg);

private:
};

#endif
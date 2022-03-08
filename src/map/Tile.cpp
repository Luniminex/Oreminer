#include "map/Tile.hpp"

//constructor / destructor

Tile::Tile(int gridPosX, int gridPosY, int gridSize, const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision)
{

	tile.setPosition(sf::Vector2f((float)gridPosX * gridSize, (float)gridPosY * gridSize));
	tile.setTexture(tileSheet);
	tile.setTextureRect(rect);

	this->collision = isCollision;
	this->type = tileTypes::tile;
}

Tile::Tile()
{
}

Tile::~Tile()
{
}

/**********************accessors*********************/

int Tile::GetID()
{
	return -1;
}

unsigned int Tile::GetUniqueID()
{
	return -1;
}

sf::IntRect Tile::GetRect()
{
	return tile.getTextureRect();
}

bool Tile::isCollision()
{
	return collision;
}

int Tile::GetType()
{
	return type;
}

sf::Vector2f Tile::GetPosition()
{
	return tile.getPosition();
}

sf::FloatRect Tile::GetGlobalBounds()
{
	return tile.getGlobalBounds();
}

/******************modifiers****************/

void Tile::ChangeTexture(const sf::IntRect& texRect)
{
	tile.setTextureRect(texRect);
}

/***************public functions********************/

void Tile::Update(float dt)
{
	(void)dt;
}

void Tile::Display(sf::RenderTarget& trg)
{
	trg.draw(tile);
}

#include "Portal.hpp"
/*****************constructor / destructor*********************/
Portal::Portal(int gridPosX, int gridPosY, int gridSize, const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int mapID, bool accessible) :
	Tile(gridPosX, gridPosY, gridSize, tileSheet, rect, isCollision)
{
	this->mapID = mapID;
	this->accessible = accessible;
	this->type = tileTypes::portal;
}

Portal::~Portal()
{
}

/*****************accessors*********************/
int Portal::GetID()
{
	return mapID;
}

bool Portal::GetAccess()
{
	return accessible;
}

bool Portal::ContainsPoint(sf::Vector2f point)
{
	if (GetGlobalBounds().contains(point))
	{
		return true;
	}
	return false;
}

/*****************modifiers*********************/
void Portal::SetMapID(int ID)
{
	mapID = ID;
}

void Portal::SetAccess(bool access)
{
	accessible = access;
}

void Portal::Update(float dt)
{
	(void)dt;
}

void Portal::Display(sf::RenderTarget& trg)
{
	trg.draw(tile);
}

/*****************public functions*********************/

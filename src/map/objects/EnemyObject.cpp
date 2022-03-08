#include "map/objects/EnemyObject.hpp"
/**********construtor / destructor************/

EnemyObject::EnemyObject(int ID, bool dead, int gridPosX, int gridPosY, int gridSize, sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int frames) :
	Tile(gridPosX, gridPosY, gridSize, tileSheet, rect, isCollision),
	anim(tile, tileSheet, 32, 32, 0, ID, frames, 0.125f),
	senseArea(sf::Vector2f(3 * gridSize, 3 * gridSize))
{
	this->type = tileTypes::enemy;
	this->ID = ID;
	this->dead = dead;
	sensing = true;
	senseArea.setPosition(tile.getGlobalBounds().left - gridSize, tile.getGlobalBounds().top - gridSize);
	if (dead == true)
	{
		tile.setTextureRect(sf::IntRect(sf::Vector2i((anim.GetFrames() + 1) * 32, 0), sf::Vector2i(32, 32)));
	}
}

EnemyObject::~EnemyObject()
{
}
/*******************Accessors***********************/
int EnemyObject::GetID()
{
	return this->ID;
}

bool EnemyObject::IsSensing()
{
	return sensing;
}

bool EnemyObject::IsInSenseArea(sf::Vector2f point)
{
	if (dead == false)
	{
		if (senseArea.getGlobalBounds().contains(point))
		{
			return true;
		}
	}
	return false;
}

bool EnemyObject::IsDead()
{
	return dead;
}

/**********************modifiers********************/

void EnemyObject::SetSense(bool toSense)
{
	sensing = toSense;
}

void EnemyObject::SetDead(bool isDead)
{
	this->dead = isDead;
	if (dead == true)
	{
		tile.setTextureRect(sf::IntRect(sf::Vector2i((anim.GetFrames() + 1) * 32, anim.currentRect.top), sf::Vector2i(32, 32)));
	}
}

/**********************public functions*******************/

void EnemyObject::Update(float dt)
{
	if (!dead)
		anim.Play(dt);
}

void EnemyObject::Display(sf::RenderTarget& trg)
{
	trg.draw(tile);
}
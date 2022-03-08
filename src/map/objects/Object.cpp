#include "map/objects/Object.hpp"

//static vars

unsigned int Object::count = 0;

//constructor / destructor
Object::Object(int id, int gridPosX, int gridPosY, int gridSize,
	const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, const std::string& name, int hardness, int resourceType, unsigned int remainingResource, sf::Vector2f destroyedTex) :
	Tile(gridPosX, gridPosY, gridSize, tileSheet, rect, isCollision)
{
	this->ID = id;
	this->uniqueID = count;
	count++;

	type = tileTypes::object;
	this->name = name;
	this->hardness = hardness;
	this->resourceType = resourceType;
	this->remainingResource = remainingResource;
	this->destroyedTex = destroyedTex;
}

Object::~Object()
{
}

/***************************accessors******************************/

bool Object::ContainsPoint(sf::Vector2f point)
{
	if (GetGlobalBounds().contains(point))
	{
		return true;
	}
	return false;
}

int Object::GetID()
{
	return ID;
}

unsigned int Object::GetUniqueID()
{
	return uniqueID;
}

const std::string& Object::GetName()
{
	return name;
}

int Object::GetResourceType()
{
	return resourceType;
}

unsigned int Object::GetRemainingResource()
{
	return remainingResource;
}

sf::Vector2f Object::GetDestroyedTextureCoords()
{
	return destroyedTex;
}

int Object::GetHardness()
{
	return this->hardness;
}

/**********************modifiers**************/

void Object::RemoveRemainingResources(int toRemove)
{
	remainingResource -= toRemove;
}

void Object::ChangeTextureWhenDestroyed()
{
	ChangeTexture(sf::IntRect(destroyedTex.x, destroyedTex.y, 32, 32));
}

void Object::Update(float dt)
{
	(void)dt;
}

void Object::Display(sf::RenderTarget& trg)
{
	trg.draw(this->tile);
}

/*********public functions**********/

#include "containers/ObjectData.hpp"
//construcotr / destructor
ObjectData::ObjectData(int ID, const std::string& name, int hardness, sf::Vector2f textureRect, int resourceType, unsigned int resourceRemaining, sf::Vector2f destroyedTex)
{
	this->ID = ID;
	this->name = name;
	this->hardness = hardness;
	this->textureRect = textureRect;
	this->resourceType = resourceType;
	this->resourceRemaining = resourceRemaining;
	this->destroyedTex = destroyedTex;
}

ObjectData::ObjectData()
{

}

ObjectData::~ObjectData()
{
}

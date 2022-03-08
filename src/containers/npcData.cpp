#include "containers/npcData.hpp"

NPCData::NPCData(int id, const std::string& name, bool hasCraftingInterface, sf::Vector2f startCoords, int frames)
{
	this->id = id;
	this->name = name;
	this->hasCraftingInterface = hasCraftingInterface;
	this->startCoords = startCoords;
	this->frames = frames;
}

#ifndef NPCDATA_HPP
#define NPCDATA_HPP

class NPCData
{

public:
	int id;
	std::string name;
	int hasCraftingInterface;
	sf::Vector2f startCoords;
	int frames;

public:
	NPCData(int id, const std::string& name, bool hasCraftingInterface, sf::Vector2f startCoords, int frames);
};

#endif
#ifndef OBJECTDATA_HPP
#define OBJECTDATA_HPP

struct ObjectData
{
	//variables
public:
	int ID;
	std::string name;
	int hardness;
	sf::Vector2f textureRect;
	int resourceType;
	unsigned int resourceRemaining;
	sf::Vector2f destroyedTex;
	//constructors
public:
	ObjectData(int ID, const std::string& name, int hardness, sf::Vector2f textureRect, int resourceType, unsigned int resourceRemaining, sf::Vector2f destroyedTex);
	ObjectData();
	~ObjectData();
};
#endif
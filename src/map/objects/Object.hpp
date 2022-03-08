#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "map/Tile.hpp"

class Tile;

class Object : public Tile
{
	//protected vars
protected:
	int ID;					   // ID assigned to object when created signaling its template
	static unsigned int count; //NO objects initiated
	int uniqueID;			   //unique ID of object so it can be found in arrays
	std::string name;
	int hardness;
	int resourceType;
	unsigned int remainingResource;

	sf::Vector2f destroyedTex;
	//variables
public:
	//constructors
public:
	Object(int id, int gridPosX, int gridPosY, int gridSize, const sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, const std::string& name, int hardness, int resourceType, unsigned int remainingResource, sf::Vector2f destroyedTex);

	virtual ~Object();

	//accessors
public:
	//if it contains, returns true else false
	bool ContainsPoint(sf::Vector2f point);

	//returns ID of object
	int GetID();

	unsigned int GetUniqueID();

	//returns name of object
	const std::string& GetName();

	//returns type of resource
	int GetResourceType();

	//returns how many resources remain
	unsigned int GetRemainingResource();

	//returns coords of destroyed texture
	sf::Vector2f GetDestroyedTextureCoords();

	//returns hardness of object
	int GetHardness();
	//modifiers
public:
	//removes amount of resources from remaining resources
	void RemoveRemainingResources(int toRemove);

	//changes texture to something else when destroyed
	void ChangeTextureWhenDestroyed();

	//functions
public:
	//updates object
	virtual void Update(float dt);

	//displays object
	virtual void Display(sf::RenderTarget& trg);
};

#endif
#ifndef NPC_HPP
#define NPC_HPP

//NPCs which will give quests to player

#include "map/objects/EnemyObject.hpp"

class EnemyObject;

class NPC : public EnemyObject
{
public:
	bool hasCraftingInterface;

	//constructors / desctructors
public:
	NPC(int ID, bool dead, bool hasCraftingInterFace, int gridPosX, int gridPosY, int gridSize, sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int frames);
	~NPC();

	//accessors
public:
	//modifiers
public:
	//public functions
public:
	bool HasCraftingInterface();

	//updates npc
	void Update(float dt);

	//renders npc to win
	void Display(sf::RenderTarget& trg);

	//private functions
public:
};

#endif
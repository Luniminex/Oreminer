#include "map/objects/NPC.hpp"

/***********************Constuctors / destructors*********************/

NPC::NPC(int ID, bool dead, bool hasCraftingInterFace, int gridPosX, int gridPosY, int gridSize, sf::Texture& tileSheet, const sf::IntRect& rect, bool isCollision, int frames) :
	EnemyObject(ID, dead, gridPosX, gridPosY, gridSize, tileSheet, rect, isCollision, frames)
{
	this->type = tileTypes::npc;
	this->hasCraftingInterface = hasCraftingInterFace;
}

NPC::~NPC()
{
}

/***********************accessors*********************/

bool NPC::HasCraftingInterface()
{
	return this->hasCraftingInterface;
}

/***********************modifiers*********************/

/***********************public functions*********************/

void NPC::Update(float dt)
{
	if (!dead)
		anim.Play(dt);
}

void NPC::Display(sf::RenderTarget& trg)
{
	trg.draw(tile);
}

/***********************private functions*********************/
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "ESC/components/AnimationComponent.hpp"
#include "ESC/components/AttributeComponent.hpp"
#include "ESC/components/BodyComponent.hpp"
#include "ESC/components/CraftingComponent.hpp"
#include "ESC/components/DialogueComponent.hpp"
#include "ESC/components/GraphicalInventoryComponent.hpp"
#include "ESC/components/HitboxComponent.hpp"
#include "ESC/components/InventoryComponent.hpp"
#include "ESC/components/LevelComponent.hpp"
#include "ESC/components/MiningComponent.hpp"
#include "ESC/components/MovementComponent.hpp"
#include "ESC/components/QuestComponent.hpp"

class Entity
{
	//variables
public:
	float keyTime; ///variables for key delay
	float keyTimeMax;
	//functions
public:
	Entity();

	virtual void Update(const float& dt, sf::Vector2i& mousePosWindow) = 0;

	virtual ~Entity();

	//true if able to press a key
	bool GetKeyTime();

	//updates key timer
	void UpdateKeyTime(float dt);
};

#endif
#ifndef ENEMYDATA_HPP
#define ENEMYDATA_HPP

#include "ESC/components/AttributeComponent.hpp"

struct AnimationData
{
	std::string name = "";
	int startx = 0;
	int starty = 0;
	int frames = 0;
	int frametime = 0;

	AnimationData(const std::string name, int startx, int starty, int frames, int frametime) :
	name(name), startx(startx), starty(starty), frames(frames), frametime(frametime)
	{
	}
};

struct ItemDrop
{
	int id = -1;		//id of item that drops
	int dropChance = 0; //chance to drop
	int min = 0;		//min amount that can drop
	int max = 0;		//max amount that can drop

	ItemDrop(int ID, int dropChance, int min, int max) :
		id(ID),
		dropChance(dropChance),
		min(min),
		max(max)
	{
	}
};

class EnemyData
{

public:
	int id = 0;
	std::string name = "";
	sf::Vector2f startCoords;
	int frames = 0;
	std::string fightsceneAnimPath;

	attributes attrs;

	std::vector<AnimationData> animationList;
	std::vector<ItemDrop> itemDropList;

public:
	EnemyData(int id, const std::string& name, int level, int experience, int maxExperience, int health, int maxHealth, int healthRegen, int mana, int maxMana, int manaRegen, int stamina, int maxStamina, int staminaRegen, int strenght, int critDamage, int critChance, int dodgeChance, int armour, sf::Vector2f startCoords, int frames, std::vector<AnimationData>& animList, std::vector<ItemDrop>& dropList, const std::string& animTexPath);
	~EnemyData();

	attributes& GetAttrs();
};

#endif

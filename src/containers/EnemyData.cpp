#include "containers/EnemyData.hpp"
/***************constructor / destructor*************/
EnemyData::EnemyData(int id, const std::string& name, int level, int experience, int maxExperience, int health, int maxHealth, int healthRegen, int mana, int maxMana, int manaRegen, int stamina, int maxStamina, int staminaRegen, int strenght, int critDamage, int critChance, int dodgeChance, int armour, sf::Vector2f startCoords, int frames, std::vector<AnimationData>& animList, std::vector<ItemDrop>& dropList, const std::string& animTexPath)
{

	this->id = id;
	this->name = name;

	attrs.atrTypes.at(ATRTYPES::level) = level;
	attrs.atrTypes.at(ATRTYPES::experience) = experience;
	attrs.atrTypes.at(ATRTYPES::experienceBar) = maxExperience;
	attrs.atrTypes.at(ATRTYPES::health) = health;
	attrs.atrTypes.at(ATRTYPES::maxHealth) = maxHealth;
	attrs.atrTypes.at(ATRTYPES::healthRegen) = healthRegen;
	attrs.atrTypes.at(ATRTYPES::mana) = mana;
	attrs.atrTypes.at(ATRTYPES::maxMana) = maxMana;
	attrs.atrTypes.at(ATRTYPES::manaRegen) = manaRegen;
	attrs.atrTypes.at(ATRTYPES::stamina) = stamina;
	attrs.atrTypes.at(ATRTYPES::maxStamina) = maxStamina;
	attrs.atrTypes.at(ATRTYPES::staminaRegen) = staminaRegen;
	attrs.atrTypes.at(ATRTYPES::strenght) = strenght;
	attrs.atrTypes.at(ATRTYPES::critDamage) = critDamage;
	attrs.atrTypes.at(ATRTYPES::critChance) = critChance;
	attrs.atrTypes.at(ATRTYPES::dodgeChance) = dodgeChance;
	attrs.atrTypes.at(ATRTYPES::armour) = armour;

	this->startCoords = startCoords;
	this->frames = frames;

	this->fightsceneAnimPath = animTexPath;

	for (auto& a : animList)
	{
		animationList.push_back(a);
	}
	for (auto& a : dropList)
	{
		itemDropList.push_back(a);
	}
}

EnemyData::~EnemyData()
{
}

attributes& EnemyData::GetAttrs()
{
	std::cout<<"getting attributes from enemy"<<std::endl;
	return attrs;
}

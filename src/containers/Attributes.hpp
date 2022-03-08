#ifndef ATTRIBUTES_HPP

#define ATTRIBUTES_HPP

#include <map>
enum ATRTYPES
{
	level,		   // level of entity
	experience,	   // experience of entity
	experienceBar, // how much experience needed to advance to next level
	miningPower,   //indicates mining power
	health,		   //indicates how much health has entity
	maxHealth,	   //maximum health
	healthRegen,   //how much health is regnerated after every turn
	mana,		   //indicates how much mana has entity
	maxMana,	   //maximum mana
	manaRegen,	   //mana regenerated per turn
	stamina,	   //current stamina
	maxStamina,	   //maximum stamina
	staminaRegen,  //stamina regenareted per turn
	strenght,	   //indicates the strength attribute of entity
	critDamage,	   //damage multiplayer
	critChance,	   //chance to activate damage multiplayer
	dodgeChance,   //chance to dodge damage
	armour		   //how much damage recieved is recieved*/
};

static std::vector<std::string> atrNameMap {
	"Level",
	"Experience",
	"Experience bar",
	"Mining power",
	"Health",
	"Max health",
	"Health regen",
	"Mana",
	"Max mana",
	"Mana regen",
	"Stamina",
	"Max stamina",
	"Stamina regen",
	"Strenght",
	"Critical damage",
	"Critical chance",
	"Dodge chance",
	"Armour",
};

struct attributes
{

	attributes();
	attributes(attributes& atrrs);

public:
	std::vector<int> atrTypes;

	void IncreaseAllBy(attributes& atrs);

	void DecreaseAllBy(attributes& atrs);

	void Init();

	void ResetAll();

	void PrintAll();

public:
	attributes operator+(attributes attrs);
	attributes operator-(attributes attrs);
};

#endif

#ifndef ATTRIBUTECOMPONENT_HPP
#define ATTRIBUTECOMPONENT_HPP

#include "containers/Attributes.hpp"
#include <rapidxml-1.13/rapidxml_ext.hpp>

#include "GUI/GUI.hpp"

//class containing attribute data of entities
class AttributeComponent
{
	//variables
private:
	attributes attrs;
	attributes equipedItemsAttrs;
	GUI::AttributeInfo attrInfo;

	//constructor / destructor
public:
	AttributeComponent(int level, int experience, int experienceBar, int miningPower, int health, int maxHealth, int healthRegen, int mana, int maxMana, int manaRegen, int stamina, int maxStamina, int staminaRegen, int strenght, int critDamage, int critChance, int dodgeChance, int armour, sf::Font& font, sf::Vector2f res);

	AttributeComponent(attributes& attribute, sf::Font& font, sf::Vector2f res);

	~AttributeComponent();
	//accesors
public:
	attributes& GetEquipedItemsAttributes();

	int& GetAttribute(int type);

	attributes& getAttributes();
	//modifiers
public:
	/*********setters***********/

	void SetAttribute(int type, int value);

	/*********setters***********/

	void UpdateAttributeInfo();

	void IncreaseEquipedAttributes(attributes& atrs);

	void DecreaseEquipedAttributes(attributes& atrs);

	void ResetEquipedAttrs();

	void IncreaseLevel(int byHowMuch = 1);

	int AddExperience(int amount);

	void AddHealth(int toAdd);

	void AddMana(int toAdd);

	void IncreaseStrenght(int byHowMuch = 1);

	void DecreaseStamina(int byHowMuch = 0);

	void RegenerateHealth();

	void RegenerateMana();

	void RegenerateStamina();

	//functions
public:
	//combines player attrs and equiped attrs
	void CombineAttributes();

	//uncombines player attrs and equiped attrs
	void UncombineAttributes();

	void SaveAttributes(const std::string& path);

	void LoadAttributes(const std::string& path);

	void Show();

	void AfterInit(sf::Vector2f res);

	void Update(float dt);

	void Display(sf::RenderTarget& trg);

private:
};

#endif
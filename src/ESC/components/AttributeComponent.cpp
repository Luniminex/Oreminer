#include "ESC/components/AttributeComponent.hpp"

/************constructors / destructor*************/

AttributeComponent::AttributeComponent(int level, int experience, int experienceBar, int miningPower, int health, int maxHealth, int healthRegen, int mana, int maxMana, int manaRegen, int stamina, int maxStamina, int staminaRegen, int strenght, int critDamage, int critChance, int dodgeChance, int armour, sf::Font& font, sf::Vector2f res) :
	attrInfo(sf::Vector2f(0.f, 20.f), sf::Vector2f(25.f, 25.f), font, res)
{
	this->attrs.atrTypes.at(ATRTYPES::level) = level;
	this->attrs.atrTypes.at(ATRTYPES::experience) = experience;
	this->attrs.atrTypes.at(ATRTYPES::experienceBar) = experienceBar;
	this->attrs.atrTypes.at(ATRTYPES::miningPower) = miningPower;
	this->attrs.atrTypes.at(ATRTYPES::health) = health;
	this->attrs.atrTypes.at(ATRTYPES::maxHealth) = maxHealth;
	this->attrs.atrTypes.at(ATRTYPES::healthRegen) = healthRegen;
	this->attrs.atrTypes.at(ATRTYPES::mana) = mana;
	this->attrs.atrTypes.at(ATRTYPES::maxMana) = maxMana;
	this->attrs.atrTypes.at(ATRTYPES::manaRegen) = manaRegen;
	this->attrs.atrTypes.at(ATRTYPES::stamina) = stamina;
	this->attrs.atrTypes.at(ATRTYPES::maxStamina) = maxStamina;
	this->attrs.atrTypes.at(ATRTYPES::staminaRegen) = staminaRegen;
	this->attrs.atrTypes.at(ATRTYPES::strenght) = strenght;
	this->attrs.atrTypes.at(ATRTYPES::critDamage) = critDamage;
	this->attrs.atrTypes.at(ATRTYPES::critChance) = critChance;
	this->attrs.atrTypes.at(ATRTYPES::dodgeChance) = dodgeChance;
	this->attrs.atrTypes.at(ATRTYPES::armour) = armour;
}

AttributeComponent::AttributeComponent(attributes& attribute, sf::Font& font, sf::Vector2f res) :
	attrInfo(sf::Vector2f(0.f, 0.f), sf::Vector2f(25.f, 25.f), font, res)
{
	std::cout << "Creating attribute Component 2" << std::endl;
	this->attrs.atrTypes = attribute.atrTypes;
	std::cout << "Created attribute Component 2" << std::endl;
}

AttributeComponent::~AttributeComponent()
{
}
/*****************accessors***************/
attributes& AttributeComponent::GetEquipedItemsAttributes()
{
	return equipedItemsAttrs;
}

int& AttributeComponent::GetAttribute(int type)
{
	return attrs.atrTypes.at(type);
}
attributes& AttributeComponent::getAttributes()
{
	return attrs;
}

/************modifiers****************/

void AttributeComponent::UpdateAttributeInfo()
{
	attrInfo.ClearAttributes();
	for (unsigned int i = 0; i < atrNameMap.size(); i++)
	{
		if (equipedItemsAttrs.atrTypes.at(i) != 0)
		{
			attrInfo.AddAttribute(atrNameMap.at(i) + ": " + std::to_string(attrs.atrTypes.at(i)) + " + " + std::to_string(equipedItemsAttrs.atrTypes.at(i)));
		}
		else
		{
			attrInfo.AddAttribute(atrNameMap.at(i) + ": " + std::to_string(attrs.atrTypes.at(i)));
		}
	}
}

void AttributeComponent::IncreaseEquipedAttributes(attributes& atrs)
{
	this->equipedItemsAttrs.IncreaseAllBy(atrs);
	std::cout << "Equiped Attributes" << std::endl;
	this->equipedItemsAttrs.PrintAll();
}

void AttributeComponent::DecreaseEquipedAttributes(attributes& atrs)
{
	this->equipedItemsAttrs.DecreaseAllBy(atrs);
	std::cout << "Equiped Attributes" << std::endl;
	this->equipedItemsAttrs.PrintAll();
}

void AttributeComponent::ResetEquipedAttrs()
{
	this->equipedItemsAttrs.ResetAll();
}

void AttributeComponent::SetAttribute(int type, int value)
{
	this->attrs.atrTypes.at(type) = value;
}

int AttributeComponent::AddExperience(int amount)
{
	if (attrs.atrTypes.at(ATRTYPES::experience) + amount >= attrs.atrTypes.at(ATRTYPES::experienceBar))
	{
		attrs.atrTypes.at(ATRTYPES::level)++;
		attrs.atrTypes.at(ATRTYPES::experience) = attrs.atrTypes.at(ATRTYPES::experience) + amount - attrs.atrTypes.at(ATRTYPES::experienceBar);
		attrs.atrTypes.at(ATRTYPES::experienceBar) = attrs.atrTypes.at(ATRTYPES::level) * 15;
		return 1;
	}
	else
	{
		attrs.atrTypes.at(ATRTYPES::experience) = attrs.atrTypes.at(ATRTYPES::experience) + amount;
		return 0;
	}
}

void AttributeComponent::IncreaseLevel(int byHowMuch)
{
	this->attrs.atrTypes.at(ATRTYPES::level) += byHowMuch;
}

void AttributeComponent::AddHealth(int toAdd)
{

	attrs.atrTypes.at(ATRTYPES::health) += toAdd;
	if (attrs.atrTypes.at(ATRTYPES::health) > attrs.atrTypes.at(ATRTYPES::maxHealth))
	{
		attrs.atrTypes.at(ATRTYPES::health) = attrs.atrTypes.at(ATRTYPES::maxHealth);
	}
}

void AttributeComponent::AddMana(int toAdd)
{
	attrs.atrTypes.at(ATRTYPES::mana) += toAdd;
	if (attrs.atrTypes.at(ATRTYPES::mana) > attrs.atrTypes.at(ATRTYPES::maxMana))
	{
		attrs.atrTypes.at(ATRTYPES::mana) = attrs.atrTypes.at(ATRTYPES::maxMana);
	}
}

void AttributeComponent::IncreaseStrenght(int byHowMuch)
{
	attrs.atrTypes.at(ATRTYPES::strenght) += byHowMuch;
}

void AttributeComponent::DecreaseStamina(int byHowMuch)
{
	if (attrs.atrTypes.at(ATRTYPES::stamina) >= 0)
	{
		if ((attrs.atrTypes.at(ATRTYPES::stamina) - byHowMuch) > 0)
		{
			attrs.atrTypes.at(ATRTYPES::stamina) -= byHowMuch;
		}
		else
		{
			attrs.atrTypes.at(ATRTYPES::stamina) = 0;
		}
	}
}

void AttributeComponent::RegenerateHealth()
{
	if (attrs.atrTypes.at(ATRTYPES::health) + attrs.atrTypes.at(ATRTYPES::healthRegen) >= attrs.atrTypes.at(ATRTYPES::maxHealth))
	{
		attrs.atrTypes.at(ATRTYPES::health) = attrs.atrTypes.at(ATRTYPES::maxHealth);
		std::cout << "Regenerated full health" << std::endl;
	}
	else
	{
		attrs.atrTypes.at(ATRTYPES::health) += attrs.atrTypes.at(ATRTYPES::healthRegen);
		std::cout << "Regenerated regen amount " << attrs.atrTypes.at(ATRTYPES::healthRegen) << std::endl;
	}
}

void AttributeComponent::RegenerateMana()
{
	if (attrs.atrTypes.at(ATRTYPES::mana) + attrs.atrTypes.at(ATRTYPES::manaRegen) >= attrs.atrTypes.at(ATRTYPES::maxMana))
	{
		attrs.atrTypes.at(ATRTYPES::mana) = attrs.atrTypes.at(ATRTYPES::maxMana);
	}
	else
	{
		attrs.atrTypes.at(ATRTYPES::mana) += attrs.atrTypes.at(ATRTYPES::manaRegen);
	}
}

void AttributeComponent::RegenerateStamina()
{
	if (attrs.atrTypes.at(ATRTYPES::stamina) + attrs.atrTypes.at(ATRTYPES::staminaRegen) >= attrs.atrTypes.at(ATRTYPES::maxStamina))
	{
		attrs.atrTypes.at(ATRTYPES::stamina) = attrs.atrTypes.at(ATRTYPES::maxStamina);
	}
	else
	{
		attrs.atrTypes.at(ATRTYPES::stamina) += attrs.atrTypes.at(ATRTYPES::staminaRegen);
	}
}

void AttributeComponent::CombineAttributes()
{
	attrs = attrs + equipedItemsAttrs;
}

void AttributeComponent::UncombineAttributes()
{
	attrs = attrs - equipedItemsAttrs;
}

void AttributeComponent::SaveAttributes(const std::string& path)
{
	std::cout << "Saving attributes" << std::endl;
	rapidxml::xml_document<> doc; // DOM object

	rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "Attributes", "attributes");
	doc.append_node(root);

	rapidxml::xml_node<>* attribute;
	rapidxml::xml_node<>* child;

	const char* value;

	attribute = doc.allocate_node(rapidxml::node_element, "attribute");
	root->append_node(attribute);

	attribute = root->first_node("attribute");
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::level)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("level"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*LEVEL*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::experience)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("experience"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*EXPERIENCE*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::experienceBar)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("experienceBar"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*EXPERIENCEBAR*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::miningPower)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("miningPower"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*EXPERIENCEBAR*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::health)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("health"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*HEALTH*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::maxHealth)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("maxhealth"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MAXHEALTH*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::healthRegen)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("healthregen"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*HEALTHREGEN*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::mana)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("mana"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MANA*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::maxMana)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("maxmana"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MAXMANA*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::manaRegen)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("manaregen"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MANAREGEN*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::stamina)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("stamina"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*STAMINA*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::maxStamina)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("maxstamina"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MAXSTAMINA*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::staminaRegen)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("staminaregen"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*MAXHEALTH*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::strenght)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("strenght"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*STRENGHT*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::critDamage)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("critdamage"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*CRIT DAMAGE*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::critChance)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("critchance"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*CRIT CHANCE*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::dodgeChance)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("dodgechance"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*DODGE CHANCE*/

	attribute->next_sibling();
	value = doc.allocate_string(std::to_string(attrs.atrTypes.at(ATRTYPES::armour)).c_str());
	child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("armour"), doc.allocate_string(std::string(value).c_str()));
	attribute->append_node(child);
	/*ARMOUR*/

	std::string xmlAsString; // string for DOM object to save in
	print(std::back_inserter(xmlAsString), doc);

	std::ofstream file_stored(path, std::ofstream::trunc); //open output file, and with trunc erase its contents
	if (!file_stored.is_open())
	{
		std::cout << "FAILED TO SAVE ATTRIBUTES" << std::endl;
		return;
	}
	file_stored << doc;	 // write to file
	file_stored.close(); //close file
	doc.clear();
}

void AttributeComponent::LoadAttributes(const std::string& path)
{
	std::cout << "loading attributes" << std::endl;
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO OPEN FILE FOR LOADING ATTRIBUTES" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("Attributes");
	if (node != 0)
	{
		node = doc.first_node("Attributes")->first_node("attribute");
		const char* temp;

		enum strings
		{
			Elevel,
			Eexperience,
			EexperienceBar,
			EminingPower,
			Ehealth,
			EmaxHealth,
			EhealthRegen,
			Emana,
			EmaxMana,
			EmanaRegen,
			Estamina,
			EmaxStamina,
			EstaminaRegen,
			Estrenght,
			EcritDamage,
			EcritChance,
			EdodgeChance,
			Earmour
		};

		std::map<std::string, int> strMap;

		strMap["level"] = strings::Elevel;
		strMap["experience"] = strings::Eexperience;
		strMap["experienceBar"] = strings::EexperienceBar;
		strMap["miningPower"] = strings::EminingPower;
		strMap["health"] = strings::Ehealth;
		strMap["maxhealth"] = strings::EmaxHealth;
		strMap["healthregen"] = strings::EhealthRegen;
		strMap["mana"] = strings::Emana;
		strMap["maxmana"] = strings::EmaxMana;
		strMap["manaregen"] = strings::EmanaRegen;
		strMap["stamina"] = strings::Estamina;
		strMap["maxstamina"] = strings::EmaxStamina;
		strMap["staminaregen"] = strings::EstaminaRegen;
		strMap["strenght"] = strings::Estrenght;
		strMap["critdamage"] = strings::EcritDamage;
		strMap["critchance"] = strings::EcritChance;
		strMap["dodgechance"] = strings::EdodgeChance;
		strMap["armour"] = strings::Earmour;

		for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
		{
			for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
			{
				switch (strMap.at(child->name()))
				{
					case strings::Elevel:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::level) = std::stoi(temp);
						break;
					case strings::Eexperience:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::experience) = std::stoi(temp);
						break;
					case strings::EexperienceBar:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::experienceBar) = std::stoi(temp);
						break;
					case strings::EminingPower:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::miningPower) = std::stoi(temp);
						break;
					case strings::Ehealth:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::health) = std::stoi(temp);
						break;
					case strings::EmaxHealth:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::maxHealth) = std::stoi(temp);
						break;
					case strings::EhealthRegen:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::healthRegen) = std::stoi(temp);
						break;
					case strings::Emana:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::mana) = std::stoi(temp);
						break;
					case strings::EmaxMana:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::maxMana) = std::stoi(temp);
						break;
					case strings::EmanaRegen:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::manaRegen) = std::stoi(temp);
						break;
					case strings::Estamina:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::stamina) = std::stoi(temp);
						break;
					case strings::EmaxStamina:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::maxStamina) = std::stoi(temp);
						break;
					case strings::EstaminaRegen:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::staminaRegen) = std::stoi(temp);
						break;
					case strings::Estrenght:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::strenght) = std::stoi(temp);
						break;
					case strings::EcritDamage:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::critDamage) = std::stoi(temp);
						break;
					case strings::EcritChance:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::critChance) = std::stoi(temp);
						break;
					case strings::EdodgeChance:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::dodgeChance) = std::stoi(temp);
						break;
					case strings::Earmour:
						temp = child->value();
						attrs.atrTypes.at(ATRTYPES::armour) = std::stoi(temp);
						break;
					default:
						break;
				}
			}
		}
	}
	doc.clear();
	int i = 0;
	attrInfo.ClearAttributes();
	for (auto& a : atrNameMap)
	{
		attrInfo.AddAttribute(a + ": " + std::to_string(attrs.atrTypes.at(i)));
		i++;
	}
}

void AttributeComponent::Show()
{
	attrInfo.shown = !attrInfo.shown;
}

void AttributeComponent::AfterInit(sf::Vector2f res)
{
	this->attrInfo.AfterInit(res);
}

void AttributeComponent::Update(float dt)
{
	attrInfo.Update(dt);
}

void AttributeComponent::Display(sf::RenderTarget& trg)
{
	attrInfo.Display(trg);
}
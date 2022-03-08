#include "containers/ItemData.hpp"

ItemData::ItemData(const std::string& name, const std::string& description, bool stackable, unsigned int stack, unsigned int count, int ID, int type, sf::Vector2f texPosition, attributes& attrs)
{
	this->name = name;
	this->description = description;
	this->stackable = stackable;
	this->stack = stack;
	this->count = count;
	this->ID = ID;
	this->type = type;
	this->texPosition = texPosition;

	this->attrs = attrs;
}

ItemData::ItemData()
{
	this->name = "blank";
	this->stackable = false;
	this->stack = 0;
	this->count = 0;
	this->ID = -1;
	this->texPosition = { 0, 0 };
}

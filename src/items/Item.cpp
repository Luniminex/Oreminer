#include "items/Item.hpp"

/***************************constructor / destructors**********************/

Item::Item(int ID, int type, const std::string& name, const std::string& description, bool stackable, unsigned int stack, unsigned int count, sf::Vector2f texPosition, attributes& attrs)
{
	data.ID = ID;
	data.type = type;
	data.name = name;
	data.description = description;
	data.stackable = stackable;
	data.stack = stack;
	data.count = count;
	data.texPosition = texPosition;
	data.attrs = attrs;
	data.equiped = false;
}

Item::Item(const Item& item)
{
	data = item.data;
}

Item::~Item()
{
}

/***************************accessors**********************/

bool Item::IsEquiped()
{
	return data.equiped;
}

const std::string& Item::GetName()
{
	return data.name;
}

bool Item::IsStackable()
{
	return data.stackable;
}

unsigned int Item::GetMaxStack()
{
	return data.stack;
}

unsigned int Item::GetCount()
{
	return data.count;
}

int Item::GetID()
{
	return data.ID;
}

sf::Vector2f Item::GetTexCoords()
{
	return data.texPosition;
}

ItemData& Item::GetItemData()
{
	/*std::cout << "Returning item data" << std::endl;
	std::cout << "item data atrs" << std::endl;*/
	for (unsigned int i = 0; i < data.attrs.atrTypes.size(); i++)
	{	//tohle n2jak nefunguje >{
		//	std::cout << atrNameMap.at(i) + ": " + std::to_string(data.attrs.atrTypes.at(i)) << std::endl;
	}
	return data;
}

/****************modifers***************/

void Item::SetEquip(bool equip)
{
	data.equiped = equip;
}

int Item::AddToCount(unsigned int count)
{
	if ((data.count + count) > data.stack)
	{											  // if not able to fit in its stack
		data.count = data.stack;				  //set max items in stack
		return (data.count + count) - data.stack; //returns NO items exceeding max stack
	}
	else
	{
		data.count += count; //adds to count
	}
	return 0;
}

int Item::RemoveFromCount(unsigned int count)
{
	if (data.count >= count)
	{ //if there is enough remove
		data.count -= count;
		return 1;
	}
	else
	{
		return 0;
	}
}

int Item::SetCount(int amount)
{
	if (static_cast<unsigned int>(amount) < data.stack)
	{
		data.count = amount;
		return 0;
	}
	else
	{
		data.count = data.stack;
		amount = amount - data.stack;
		return amount;
	}
}

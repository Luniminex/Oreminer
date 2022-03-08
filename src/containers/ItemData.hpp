#ifndef ITEMDATA_HPP
#define ITEMDATA_HPP

#include "containers/Attributes.hpp"

struct attributes;

enum ItemType
{
	material,
	pickaxe,
	sword,
	ring,
	necklace,
	helmet,
	gloves,
	bodyArmour,
	legArmour,
	boots
};

struct ItemData
{

public:
	std::string name;		  //jméno předmětu
	std::string description;  //popis předmětu item
	bool stackable;			  //jestli jde předmět stackovat
	unsigned int stack;		  //maximální počet předmětů v jednom stacku
	unsigned int count;		  //počet předmětů ve stacku
	int ID;					  //identifikátor předmětu
	sf::Vector2f texPosition; //drží pozici souřadnic v textuře
	bool equiped;			  //udává, jestli je předmět nasazený
	int type;				  //jaký typ předmětu to je
	attributes attrs;		  //pokud jde nasadit, tak má nastavené určité hodnoty atributů
public:
	ItemData(const std::string& name, const std::string& descriptionn, bool stackable, unsigned int stack, unsigned int count, int ID, int type, sf::Vector2f texPosition, attributes& attrs);
	ItemData();
};

#endif
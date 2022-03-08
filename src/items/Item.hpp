#ifndef ITEMS_HPP
#define ITEMS_HPP

#include "containers/ItemData.hpp"

class Item
{
	//variables
private:
	ItemData data;

	//constructors
public:
	Item(int ID, int type, const std::string& name, const std::string& description,  bool stackable, unsigned int stack, unsigned int count, sf::Vector2f texPosition, attributes& atrrs);

	//copy constructor
	Item(const Item& item);

	~Item();

	//accessors
public:
	//returns name of the this item
	const std::string& GetName();

	//returns true if is equiped, else false
	bool IsEquiped();

	//returns true if is stackble, false if not
	bool IsStackable();

	//returns max number of items that can be stacked
	unsigned int GetMaxStack();

	//returns count
	unsigned int GetCount();

	//returns ID of item
	int GetID();

	//returns texture coords of texture
	sf::Vector2f GetTexCoords();

	//returns all data about item
	ItemData& GetItemData();

	//modifiers
public:
	void SetEquip(bool equip);

	//adds to count and checks if there is space in stack if not, returns number of items not able to fit into the stack
	int AddToCount(unsigned int count);

	/*removes from count
	*	Returns 1 if success
	*	Returns 0 if failed
	*/
	int RemoveFromCount(unsigned int count);

	/*sets amount of items
	*	Returns 0 if all of amount was added and no residue is left
	*	Else returns residue of amount
	*/
	int SetCount(int amount);
	//functions
public:
};

#endif
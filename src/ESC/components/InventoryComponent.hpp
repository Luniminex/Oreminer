#ifndef INVENTORYCOMPONENT_HPP
#define INVENTORYCOMPONENT_HPP

#include <fstream>
#include <rapidxml-1.13/rapidxml_ext.hpp>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "gui/GUI.hpp"
#include "items/Item.hpp"

class Item;

class InventoryComponent
{
private:
	int itemCount;							  //počet předmětů
	std::vector<std::unique_ptr<Item>> items; //vektor předmětů
	std::map<int, Item*> equipedItems;		  //ukazatele na předměty, které jsou zrovna nasazeny
	bool updated;							  // jestli je inventář aktualizován
	attributes blankAttrs;					  //proměnná pro prázdné atributy předmětů
	Item blank;								  //prázdný předmět

	//constructors
public:
	InventoryComponent();

	//copy constructor
	InventoryComponent(InventoryComponent& inv);
	~InventoryComponent();

	//accessors
public:
	std::map<int, Item*> GetEquipeditems();

	//returns item at index
	Item& GetItemAt(unsigned int index);

	//returns size of vector
	unsigned int GetSizeOfVector();

	//returns vector of items
	std::vector<std::unique_ptr<Item>>& GetInventory();

	//returns updated
	bool IsUpdated();

	bool HasItem(int id, int count);

	//modifiers
public:
	int EquipItem(Item& item);

	int UnequipItem(int type);

	int EquipItem(Item& item, int index, GUI::Listbox& list);

	int UnequipItem(int type, GUI::Listbox& list);

	//pushes back item
	void GiveItem(int ID, int type, const std::string& name, const std::string& description, bool stackable, unsigned int count, unsigned int stack, sf::Vector2f texCoords, attributes& attrs);

	//pushes back item
	void GiveItem(Item& item);

	//pushes back item with specified amount of items
	void GiveItem(Item& item, int amount);
	/*removes item by ID
	*
	* Returns 1 if success
	* Returns 0 if there is not enough items in inventory to remove
	*
	*/
	int RemoveItemByID(int ID, unsigned int amount = 1);

	/* removes whole stack if items at index
	*
	*Returns 1 if success
	*Returns 0 if fail
	*
	*/
	int RemoveStackAtIndex(int index);

	//adds item to index, if it exceeds its max stack / nonstackable pushes it to back
	//void AddItem(unsigned int index);

	//functions
public:
	//counts NO items inside invetory
	int CountItems();

	//sets updated to false
	void Changed();

	//sets updated to true
	void Update();

	//saves items to file
	void SaveInventory(const std::string& path);

	//loads items to inventory from file
	void LoadInventory(const std::string& path, std::map<int, std::unique_ptr<Item>>& itemTemplates);

	//clears all items from inventory
	void ClearInventory();
};

#endif

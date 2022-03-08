#include "ESC/components/InventoryComponent.hpp"

/*******************************constructors / destructors******************************************/

InventoryComponent::InventoryComponent() :
	blankAttrs(),
	blank(-1, -1, "blank", "blank", 0, 0, 0, sf::Vector2f(-1.f, -1.f), blankAttrs)
{
	updated = false;
	blank.GetItemData().attrs.Init();
	blank.SetEquip(true);
	equipedItems[ItemType::material] = &blank;
	equipedItems[ItemType::pickaxe] = &blank;
	equipedItems[ItemType::sword] = &blank;
	equipedItems[ItemType::ring] = &blank;
	equipedItems[ItemType::necklace] = &blank;
	equipedItems[ItemType::helmet] = &blank;
	equipedItems[ItemType::bodyArmour] = &blank;
	equipedItems[ItemType::legArmour] = &blank;
	equipedItems[ItemType::boots] = &blank;
	equipedItems[ItemType::gloves] = &blank;
}

InventoryComponent::InventoryComponent(InventoryComponent& inv) :
	blank(-1, -1, "blank", "blank", 0, 0, 0, sf::Vector2f(-1.f, -1.f), blankAttrs)
{
	for (auto& a : inv.GetInventory())
	{
		GiveItem(*a);
	}
	this->updated = inv.updated;
	equipedItems = inv.equipedItems;
}

InventoryComponent::~InventoryComponent()
{
}

/*******************************accessors******************************************/

Item& InventoryComponent::GetItemAt(unsigned int index)
{
	return *items.at(index).get();
}

unsigned int InventoryComponent::GetSizeOfVector()
{
	return items.size();
}

std::vector<std::unique_ptr<Item>>& InventoryComponent::GetInventory()
{
	return items;
}

bool InventoryComponent::IsUpdated()
{
	return updated;
}

bool InventoryComponent::HasItem(int id, int count)
{
	int totalCount = 0;
	for (auto& a : items)
	{
		if (a->GetID() == id)
		{
			totalCount += a->GetCount();
			if (totalCount >= count)
			{
				return true;
			}
		}
	}
	return false;
}

/*******************************modifiers******************************************/

std::map<int, Item*> InventoryComponent::GetEquipeditems()
{
	return equipedItems;
}

int InventoryComponent::EquipItem(Item& item)
{
	if (item.GetItemData().type != ItemType::material)
	{
		equipedItems.at(item.GetItemData().type) = &item;
		item.SetEquip(true);
		std::cout << "Equiped " << item.GetName() << std::endl;
		for (auto& a : equipedItems)
		{
			std::cout << "Equiped items: " << a.second->GetName() << std::endl;
		}
		updated = true;
		return true;
	}
	return false;
}

int InventoryComponent::UnequipItem(int type)
{
	if (equipedItems.at(type)->GetID() != -1)
	{
		std::cout << "Unequiped " << equipedItems.at(type)->GetName();
		equipedItems.at(type)->SetEquip(false);
		equipedItems.at(type) = &blank;
		for (auto& a : equipedItems)
		{
			std::cout << "Equiped items: " << a.second->GetName() << std::endl;
		}
		updated = true;
		return true;
	}
	return false;
}

int InventoryComponent::EquipItem(Item& item, int index, GUI::Listbox& list)
{

	if (item.GetItemData().type != ItemType::material)
	{
		if (equipedItems.at(item.GetItemData().type)->GetID() == -1)
		{
			equipedItems.at(item.GetItemData().type) = &item;
			item.SetEquip(true);
		}
		else
		{
			for (auto& a : list.GetList())
			{
				if (a->GetItemData().type == item.GetItemData().type && a->GetItemData().equiped == true)
				{
					a->GetItemData().equiped = false;
				}
			}
			equipedItems.at(item.GetItemData().type) = &item;
			item.SetEquip(true);
			list.GetElementAt(index).GetItemData().equiped = true;
		}
		std::cout << "Equiped " << item.GetName() << std::endl;
		for (auto& a : equipedItems)
		{
			std::cout << "Equiped items: " << a.second->GetName() << std::endl;
		}
		updated = true;
		return true;
	}
	return false;
}

void InventoryComponent::GiveItem(int ID, int type, const std::string& name, const std::string& description, bool stackable, unsigned int count, unsigned int stack, sf::Vector2f texCoords, attributes& attrs)
{
	items.push_back(std::unique_ptr<Item>(new Item(ID, type, name, description, stackable, stack, count, texCoords, attrs)));
	Changed();
}

void InventoryComponent::GiveItem(Item& item)
{

	bool endLoop = false;
	bool addItem = false;

	if (items.size() == 0)
	{ //pokud je inventář prázdný, tak se vytvoří nový předmět
		items.push_back(std::unique_ptr<Item>(new Item(item)));
		Changed();
	}
	else
	{
		std::cout << "Size not 0" << std::endl;
		for (auto& a : items)
		{ //projde celým inventářem
			if (a->GetID() == item.GetID())
			{ //pokud najde předmět se stejným identifikátorem
				if (a->IsStackable())
				{ //pokud jde předmět vrstvit
					if (a->GetCount() < a->GetMaxStack())
					{									//pokud předmětů méně než je dovoleno ve stacku
						a->AddToCount(item.GetCount()); //přidá se počet do předmětu
						addItem = false;
						endLoop = true;
						Changed();
					}
					else
					{ //je více předmětů než je dovoleno
						addItem = true;
					}
				}
				else
				{ //item nelze vrstvit, vytvoří se nový a ukončí se smyčka
					items.push_back(std::unique_ptr<Item>(new Item(item)));
					addItem = false;
					endLoop = true;
					Changed();
				}
			}
			else
			{ //pokud není stejný identifikátor
				addItem = true;
			}
			if (endLoop)
			{ //skončí smyčku
				break;
			}
		}
		if (addItem)
		{ // pokud nebylo místo, vytvoří se nový
			std::cout << "Item count: " << item.GetCount() << std::endl;
			items.push_back(std::unique_ptr<Item>(new Item(item)));
			Changed();
			std::cout << "Gave item: " << item.GetName() << std::endl;
		}
	}
}

void InventoryComponent::GiveItem(Item& item, int amount)
{
	bool endLoop = false;
	bool addItem = false;

	//std::cout << "Item has count of" << item.GetCount() << std::endl;
	unsigned int temporaryCount = item.GetCount();
	unsigned int leftover = 0;
	item.SetCount(amount);
	/*nefunguje amount / nějak to pojebe templateItem a */
	if (items.size() == 0)
	{ //if inventory is empty, push the item to vector
		items.push_back(std::unique_ptr<Item>(new Item(item)));
		Changed();
	}
	else
	{

		for (auto& a : items)
		{ //goes through whole invenvtory
			if (a->GetID() == item.GetID())
			{ // if its same id
				if (a->IsStackable())
				{ //if the item is stackable
					if (a->GetCount() < a->GetMaxStack())
					{ //if there is space in stack
						if (a->GetCount() + item.GetCount() > a->GetMaxStack())
						{ //po přidání by furt něco zbylo co by se mělo přidat
							leftover = (a->GetCount() + item.GetCount()) - a->GetMaxStack();
							a->SetCount(a->GetMaxStack());
							item.SetCount(leftover);
							addItem = true;
						}
						else
						{
							a->SetCount(a->GetCount() + item.GetCount());
							addItem = false;
							endLoop = true;
							Changed();
						}
						if (item.GetCount() == 0)
						{
							addItem = false;
							endLoop = true;
							Changed();
						}
					}
					else
					{ //there is no space
						addItem = true;
					}
				}
				else
				{ //if item is not stackable just create it and end loop
					items.push_back(std::unique_ptr<Item>(new Item(item)));
					endLoop = true;
					Changed();
				}
			}
			else
			{ //if not same ID
				addItem = true;
			}
			if (endLoop)
			{ //goes out of a loop
				break;
			}
		}
		if (addItem)
		{ // if there wasn't any space in any of the stacks in inventory, create new item
			items.push_back(std::unique_ptr<Item>(new Item(item)));
			Changed();
		}
	}
	item.SetCount(temporaryCount);
}

int InventoryComponent::RemoveItemByID(int ID, unsigned int amount)
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		auto& a = items.at(i);
		if (a->GetID() == ID)
		{ //found item
			if (a->GetCount() >= amount)
			{ //has enough items to remove
				if (a->RemoveFromCount(amount) == true)
				{ //removal was succesfull
					if (a->GetCount() == 0)
					{ //stack needs to be removed
						items.erase(items.begin() + i);
						Changed();
						return 1;
					}
					else
					{
						std::cout << amount << " was substracted and now has " << a->GetCount() << std::endl;
						Changed();
						return 1;
					}
				}
				else
				{
					return 0;
					std::cout << "Item wasnt able to substract" << std::endl;
				}
			}
			else
			{
				int temp = a->GetCount();
				amount -= temp;
				a->RemoveFromCount(temp);
				items.erase(items.begin() + i);
				Changed();
				std::cout << "item found but doesnt have enough items to substract" << std::endl;
			}
		}
	}
	return 0;
	std::cout << "Didn't find any item matching ID to remove" << std::endl;
}

int InventoryComponent::RemoveStackAtIndex(int index)
{
	if (index < static_cast<int>(items.size()))
	{ //index is smaller then size
		std::cout << "Removed whole stack of " << items.at(index)->GetName() << std::endl;
		items.erase(items.begin() + index);
		return 1;
	}
	std::cout << "Index is bigger than size of list items";
	return 0;
}

/*******************functions********************************/

int InventoryComponent::CountItems()
{
	itemCount = 0;
	itemCount = items.size();
	return itemCount;
}

void InventoryComponent::Changed()
{
	updated = false;
}

void InventoryComponent::Update()
{
	updated = true;
}

void InventoryComponent::SaveInventory(const std::string& path)
{
	std::cout << "Saving players inventory" << std::endl;
	rapidxml::xml_document<> doc; // DOM object

	rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);
	if (items.size() > 0)
	{
		rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "Items", "items");
		doc.append_node(root);

		rapidxml::xml_node<>* item;
		rapidxml::xml_node<>* child;

		const char* id;
		const char* amount;
		const char* equiped;

		for (unsigned int i = 0; i < items.size(); i++)
		{
			child = doc.allocate_node(rapidxml::node_element, "item");
			root->append_node(child);
			if (i == 0)
			{
				item = root->first_node("item");
			}
			else
			{
				item = item->next_sibling();
			}

			id = doc.allocate_string(std::to_string(items.at(i)->GetID()).c_str());
			amount = doc.allocate_string(std::to_string(items.at(i)->GetCount()).c_str());
			equiped = doc.allocate_string(std::to_string(items.at(i)->GetItemData().equiped).c_str());

			child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("id"), doc.allocate_string(std::string(id).c_str()));
			item->append_node(child);
			child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("amount"), doc.allocate_string(std::string(amount).c_str()));
			item->append_node(child);
			child = doc.allocate_node(rapidxml::node_element, doc.allocate_string("equiped"), doc.allocate_string(std::string(equiped).c_str()));
			item->append_node(child);
		}
	}
	std::string xmlAsString; // string for DOM object to save in
	print(std::back_inserter(xmlAsString), doc);

	std::ofstream file_stored(path, std::ofstream::trunc); //open output file, and with trunc erase its contents
	if (!file_stored.is_open())
	{
		std::cout << "FAILED TO SAVE ITEMS" << std::endl;
		return;
	}
	file_stored << doc;	 // write to file
	file_stored.close(); //close file
	doc.clear();
}

void InventoryComponent::LoadInventory(const std::string& path, std::map<int, std::unique_ptr<Item>>& itemTemplates)
{
	std::cout << "loading items into players inventory" << std::endl;
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO OPEN FILE FOR LOADING INVENTORY" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("Items");
	if (node != 0)
	{
		node = doc.first_node("Items")->first_node("item");
		const char* temp;

		int amount = 0;
		int ID;
		bool equiped = false;

		for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
		{
			for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
			{
				if (std::string(child->name()) == "id")
				{
					temp = child->value();
					ID = std::stoi(temp);
				}
				else if (std::string(child->name()) == "amount")
				{
					temp = child->value();
					amount = std::stoi(temp);
				}
				else if (std::string(child->name()) == "equiped")
				{
					temp = child->value();
					equiped = std::stoi(temp);
				}
				else
				{
					std::cout << "no items" << std::endl;
				}
			}
			GiveItem(*itemTemplates[ID], amount);
			if (equiped)
			{
				items.at(items.size() - 1)->SetEquip(true);
				EquipItem(*items.at(items.size() - 1));
				updated = false;
			}
		}
	}
	doc.clear();
}

void InventoryComponent::ClearInventory()
{
	items.clear();
}
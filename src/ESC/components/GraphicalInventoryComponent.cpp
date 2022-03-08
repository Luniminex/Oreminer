#include "ESC/components/GraphicalInventoryComponent.hpp"

/***********************constructors / destructors ********************************/

GraphicalInventoryComponent::GraphicalInventoryComponent(float posXPerc, float posYPerc, unsigned int NOElementsShown,
	sf::Font& font, sf::Texture& texSheet, sf::Vector2f sizePerc, InventoryComponent& invComponent, sf::Vector2f res) :
	listbox(posXPerc, posYPerc, NOElementsShown, font, texSheet, sizePerc, res),
	temporarySolution("blank", "blank", false, 0, 0, -1, 0, sf::Vector2f(0, 0), attrTemp),
	descBox(sf::Vector2f(25, 25), sf::Vector2f(25.f, 25.f), texSheet, sf::IntRect(0, 0, 0, 0), font, temporarySolution, res),
	invComponent(invComponent)
{
	listbox.Show();
	descBox.SetPosition(sf::Vector2f(listbox.GetBounds().left + listbox.GetBounds().width, listbox.GetBounds().top));
	lastBtnClicked = 0;
	NOItems = 0;
	this->NOElementsShown = NOElementsShown;
}

GraphicalInventoryComponent::~GraphicalInventoryComponent()
{
}

/*************************modifiers*************************/

void GraphicalInventoryComponent::Show()
{
	listbox.Show();
	descBox.Hide();
}

/*****************************public functions****************************/
int GraphicalInventoryComponent::GetNOLoadedItems()
{
	return NOItems;
}

void GraphicalInventoryComponent::AfterInit(sf::Vector2f res)
{
	listbox.AfterInit(res);
	descBox.AfterInit(res);
}

int GraphicalInventoryComponent::Update(sf::Vector2i mousePosWindow, float dt)
{
	listbox.Update(mousePosWindow, dt);
	descBox.Update(mousePosWindow, dt);

	if (UpdateInput() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GraphicalInventoryComponent::ClearInventory()
{
	listbox.Clear();
}

void GraphicalInventoryComponent::LoadInventory()
{
	NOItems = 0;
	for (std::unique_ptr<Item>& a : this->invComponent.GetInventory()) // iterates through all items and loads them
	{
		listbox.PushBackElement(a->GetItemData());
		NOItems++;
	}
	if (static_cast<unsigned>(NOItems) > NOElementsShown)
	{
		listbox.ShowControlBtns(true);
	}
	else
	{
		listbox.ShowControlBtns(false);
	}
}

void GraphicalInventoryComponent::Display(sf::RenderTarget& trg)
{
	listbox.Display(trg);
	descBox.Display(trg);
}

/***********private functions *****************/
int GraphicalInventoryComponent::UpdateInput()
{
	int changeMade = false;
	if (listbox.UpdateInput())
	{
		lastBtnClicked = listbox.GetInputIndex();
		descBox.LoadItem(listbox.GetInput());
		descBox.RemoveAllButtons();
		descBox.AddButton("Drop");
		if (descBox.GetItemData().type != ItemType::material)
		{
			if (descBox.GetItemData().equiped == true)
			{
				descBox.AddButton("Unequip");
			}
			else
			{
				descBox.AddButton("Equip");
			}
		}
		descBox.Show(true);
		std::cout << "Item clicked:  " << std::endl;
		descBox.GetItemData().attrs.PrintAll();
	}
	if (descBox.IsShown() == true)
	{
		if (descBox.GetButton(0).GetState() == btnStates::BTN_PRESSED && descBox.GetKeyTimer() == true)
		{
			descBox.ResetStateBtns();
			descBox.Hide();
		}

		if (descBox.GetButton(1).GetState() == btnStates::BTN_PRESSED && descBox.GetKeyTimer() == true)
		{
			if (invComponent.GetItemAt(lastBtnClicked).GetItemData().type != ItemType::material)
			{
				if (invComponent.GetItemAt(lastBtnClicked).GetItemData().equiped == true)
				{
					invComponent.UnequipItem(invComponent.GetItemAt(lastBtnClicked).GetItemData().type);
				}
			}
			invComponent.RemoveStackAtIndex(lastBtnClicked);
			//invComponent.RemoveItemByID(descBox.GetItemData().ID, 1);
			ClearInventory();
			LoadInventory();
			descBox.ResetStateBtns();
			descBox.Hide();
			changeMade = true;
		}
		if (descBox.GetButton(2).GetState() == btnStates::BTN_PRESSED && descBox.GetKeyTimer() == true)
		{
			if (descBox.GetItemData().equiped == true)
			{
				if (invComponent.UnequipItem(descBox.GetItemData().type) == true)
				{
					for (auto& a : listbox.GetList())
					{
						if (a->GetItemData().type == descBox.GetItemData().type)
						{
							a->ChangeRightSpecial("");
						}
					}
					listbox.GetElementAt(lastBtnClicked).GetItemData().equiped = false;
					listbox.GetElementAt(lastBtnClicked).ChangeRightSpecial("");
					descBox.Show(false);
					changeMade = true;
				}
			}
			else
			{
				std::cout << "Trying to equip" << std::endl;

				if (invComponent.EquipItem(invComponent.GetItemAt(lastBtnClicked), lastBtnClicked, listbox) == true)
				{
					for (auto& a : listbox.GetList())
					{
						if (a->GetItemData().type == descBox.GetItemData().type)
						{
							a->ChangeRightSpecial("");
						}
					}
					listbox.GetElementAt(lastBtnClicked).GetItemData().equiped = true;
					listbox.GetElementAt(lastBtnClicked).ChangeRightSpecial("Equiped");
					descBox.Show(false);
					changeMade = true;
				}
			}
		}
	}
	return changeMade;
}
#ifndef GRAPHICALINVENTORYCOMPONENT_HPP
#define GRAPHICALINVENTORYCOMPONENT_HPP

#include "ESC/components/InventoryComponent.hpp"
#include "GUI/GUI.hpp"

class InventoryComponent;

class GraphicalInventoryComponent
{

public:
	attributes attrTemp;
	GUI::Listbox listbox;
	ItemData temporarySolution; //temporary solution
	GUI::DescriptionBox descBox;
	InventoryComponent& invComponent;

	int NOItems;				  //NO items loaded in this GUI comp
	int lastBtnClicked;			  //saves index of listbox which displaybox was clicked last
	unsigned int NOElementsShown; //count of how many elements are able to be shown without scrolling

private:
	//constructors
public:
	GraphicalInventoryComponent(float posXPerc, float posYPerc, unsigned int NOElementsShown,
		sf::Font& font, sf::Texture& texSheet, sf::Vector2f sizePerc, InventoryComponent& invComponent, sf::Vector2f res);
	~GraphicalInventoryComponent();

	//modifiers
public:
	//clears inventorz
	void ClearInventory();

	//refreshes items in inventory
	void LoadInventory();

	//opens / closes inventory
	void Show();

	//functions
public:
	int GetNOLoadedItems();

	//resets objects
	void AfterInit(sf::Vector2f res);

	//updates objects
	int Update(sf::Vector2i mousePosWindow, float dt);

	//displays inventory
	void Display(sf::RenderTarget& trg);

	//private functions
private:
	//updates input
	int UpdateInput();
};

#endif
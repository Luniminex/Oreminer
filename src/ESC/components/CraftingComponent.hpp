#ifndef CRAFTINGCOMPONENT_HPP
#define CRAFTINGCOMPONENT_HPP

#include "GUI/GUI.hpp"
#include "InventoryComponent.hpp"
#include "containers/Recipe.hpp"
#include "items/Item.hpp"
#include <rapidxml-1.13/rapidxml_ext.hpp>

class Recipe;
class CraftingInterface;
class CraftBox;

class CraftingComponent
{

public:
	InventoryComponent& InvComp;						 //reference na komponent inventáře, který bude manipulovat
	sf::Texture& itemTexture;							 //reference na texturu předmětů
	ItemData tempData;									 //dočasná data
	GUI::CraftingInferface craftface;					 //grafické rozhraní pro vyrábění předmětů
	std::map<int, std::vector<Recipe>>& recipes;		 //reference na mapu receptů
	std::map<int, std::unique_ptr<Item>>& itemTemplates; //reference mapu šablon předmětů
	bool hasRecipes;									 //jestli má recepty nebo nemá

public:
	CraftingComponent(InventoryComponent& InvComp, sf::Texture& itemText, std::map<int, std::vector<Recipe>>& recipes, std::map<int, std::unique_ptr<Item>>& itemTemplates, sf::Font& font, sf::Vector2f res);
	~CraftingComponent();

public:
	void SetHasRecipes(bool hasRecipes);

	std::map<int, std::unique_ptr<Item>>& GetTemplates();

	bool GetHasRecipes();

	void Show(bool shown);

	bool IsShown();

	void SetRecipes(int id);

	void AfterInit(sf::Vector2f res);

	void Update(sf::Vector2i mousePosWindow, float dt);

	void Display(sf::RenderTarget& trg);
};

#endif
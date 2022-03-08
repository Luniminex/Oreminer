#include "CraftingComponent.hpp"

CraftingComponent::CraftingComponent(InventoryComponent& InvComp, sf::Texture& itemText, std::map<int, std::vector<Recipe>>& recipes, std::map<int, std::unique_ptr<Item>>& itemTemplates, sf::Font& font, sf::Vector2f res) :
	InvComp(InvComp),
	itemTexture(itemText),
	tempData(),
	craftface(sf::Vector2f(25.f, 25.f), sf::Vector2f(50.f, 50.f), tempData, itemTexture, font, res),
	recipes(recipes),
	itemTemplates(itemTemplates)
{
	hasRecipes = false;
}

CraftingComponent::~CraftingComponent()
{
}

void CraftingComponent::SetHasRecipes(bool hasRecipes)
{
	this->hasRecipes = hasRecipes;
}

std::map<int, std::unique_ptr<Item>>& CraftingComponent::GetTemplates()
{
	return itemTemplates;
}

bool CraftingComponent::GetHasRecipes()
{
	return this->hasRecipes;
}

bool CraftingComponent::IsShown()
{
	return craftface.IsShown();
}

void CraftingComponent::Show(bool shown)
{
	if (hasRecipes)
	{
		craftface.setShown(shown);
	}
}

void CraftingComponent::SetRecipes(int id)
{
	craftface.ClearAll(); //vyčištění všech receptů z minulého použití

	std::vector<sf::Vector2f> texCoords; //dočasný vektor pro ukládání souřadnic textur
	std::vector<int> NOItemsNeeded;		 //dočasný vektor pro ukládání počtu předmětů
	std::vector<int> ingredientsID;		 //dočasný vektor pro ukládání identifikátorů předmětů

	for (auto& a : recipes.at(id))
	{ //projede všemi recepty, které jsou určené pro dané NPC
		for (auto& b : a.ingredients)
		{															  //projede všechny ingredience receptu
			texCoords.push_back(itemTemplates.at(b)->GetTexCoords()); //vloží do vektoru souřadnice daného předmětu
		}
		texCoords.push_back(itemTemplates.at(a.result)->GetTexCoords()); //vloží do vektoru souřadnice výsledného předmětu

		NOItemsNeeded.resize(texCoords.size()); //udá vektoru velikost podle velikosti vektoru souřadnic textury
		for (unsigned int i = 0; i < a.amounts.size(); i++)
		{										   //projede vektor počtu potřebných předmětů
			NOItemsNeeded.at(i) = a.amounts.at(i); //vloží na pozici i počet potřebných předmětů z pozice i
		}

		for (auto& b : a.ingredients)
		{								//projede vektor identifikátorů předmětů
			ingredientsID.push_back(b); //vloží identifikátor předmětu do vektoru
		}

		//přidá jeden grafický recept
		craftface.AddCraftBox(texCoords, NOItemsNeeded, ingredientsID, a.result);

		//vyčistí se dočasné vektory pro další iteraci smyčky
		texCoords.clear();
		NOItemsNeeded.clear();
		ingredientsID.clear();
	}
}

void CraftingComponent::AfterInit(sf::Vector2f res)
{
	craftface.AfterInit(res);
}

void CraftingComponent::Update(sf::Vector2i mousePosWindow, float dt)
{
	craftface.CheckInput();

	craftface.Update(mousePosWindow, dt);
	if (craftface.NeedsToRefresh() == true)
	{
		craftface.LoadItem(itemTemplates.at(craftface.GetSelectedResult())->GetItemData());
	}
	if (craftface.TriedCraft() == true)
	{
		bool hasAllItems = true;

		for (unsigned int i = 0; i < craftface.GetSelectedIngredients().size(); i++)
		{
			if (InvComp.HasItem(craftface.GetSelectedIngredients().at(i), craftface.GetSelectedAmounts().at(i)) == false)
			{
				hasAllItems = false;
			}
		}

		if (hasAllItems == true)
		{
			for (unsigned int i = 0; i < craftface.GetSelectedIngredients().size(); i++)
			{
				InvComp.RemoveItemByID(craftface.GetSelectedIngredients().at(i), craftface.GetSelectedAmounts().at(i));
			}
			InvComp.GiveItem(*itemTemplates.at(craftface.GetSelectedResult()));
		}
	}
	//craftbox->Update(mousePosWindow);
}

void CraftingComponent::Display(sf::RenderTarget& trg)
{
	craftface.Display(trg);
	//craftbox->Display(trg);
}

#include "GUI/GUI.hpp"

GUI::CraftingInferface::CraftingInferface(sf::Vector2f posPerc, sf::Vector2f sizePerc, ItemData& itemdata, sf::Texture& itemTexture, sf::Font& font, sf::Vector2f res) :
	TexForBoxes(itemTexture),
	font(font),
	descBox(sf::Vector2f(posPerc.x + sizePerc.x / 2, posPerc.y), sf::Vector2f(sizePerc.x / 2, sizePerc.y), itemTexture, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)), font, itemdata, res),
	nextPage(posPerc.x, posPerc.y, sizePerc, font, "Next Page", btnTypes::basic, res),
	previousPage(posPerc.x, posPerc.y, sizePerc, font, "Previus Page", btnTypes::basic, res)
{

	keyTime = 0.f;
	keyTimeMax = 10.f;

	activePage = 0;
	crafts.resize(1);
	NoCrafts = 0;
	NOPages = 0;
	currentPage = 0;
	craftBoxSelected = -1;
	selectedResult = -1;
	needsRefresh = false;

	this->posPerc = posPerc;
	this->sizePerc = sizePerc;
	this->res = res;

	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	this->box.setSize(size);
	this->box.setPosition(position);
	this->box.setFillColor(sf::Color(255, 255, 255, 0));

	descBox.AddButton("Craft");
	descBox.GetButton(1).SetShown(true);

	descBox.Show(false);
	shown = false;

	nextPage.SetSize(size.x / 5, size.y / 7);
	nextPage.SetPosition(position.x + size.x / 2 - nextPage.GetGlobalBounds().width, position.y + size.y - nextPage.GetGlobalBounds().height);
	previousPage.SetSize(size.x / 5, size.y / 7);
	previousPage.SetPosition(position.x, position.y + size.y - nextPage.GetGlobalBounds().height);

	pageIndicator.setFont(font);
	pageIndicator.setCharacterSize(CaclCharSize(res));
	pageIndicator.setString("0/0");
	pageIndicator.setPosition(position.x + size.x / 4 - pageIndicator.getGlobalBounds().width / 2, position.y + size.y - nextPage.GetGlobalBounds().height / 2);
}

GUI::CraftingInferface::~CraftingInferface()
{
}

bool GUI::CraftingInferface::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}
bool GUI::CraftingInferface::IsShown()
{
	return shown;
}

bool GUI::CraftingInferface::NeedsToRefresh()
{
	return needsRefresh;
}
int GUI::CraftingInferface::GetSelectedResult()
{
	return selectedResult;
}

bool GUI::CraftingInferface::TriedCraft()
{
	if (descBox.GetButton(1).GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
	{
		return true;
	}
	return false;
}

std::vector<int> GUI::CraftingInferface::GetSelectedIngredients()
{
	return selectedIngredients;
}

std::vector<int> GUI::CraftingInferface::GetSelectedAmounts()
{
	return selectedAmounts;
}
void GUI::CraftingInferface::setShown(bool shown)
{
	this->shown = shown;
	nextPage.SetShown(true);
	previousPage.SetShown(true);
}

void GUI::CraftingInferface::LoadItem(ItemData& data)
{
	needsRefresh = false;
	descBox.LoadItem(data);
	descBox.Show(true);
}

void GUI::CraftingInferface::ClearAll()
{
	activePage = 0;
	crafts.resize(1);
	NoCrafts = 0;
	NOPages = 0;
	currentPage = 0;
	for (auto& a : crafts)
	{
		a.clear();
	}
	crafts.clear();
	pageIndicator.setString(std::to_string(activePage) + "/" + std::to_string(NOPages));
}

void GUI::CraftingInferface::AddCraftBox(std::vector<sf::Vector2f> texCoords, std::vector<int> NOitemsNeeded, std::vector<int> ingredientsID, int resultID)
{
	NoCrafts++;
	currentPage = (NoCrafts - 1) / 6;
	if (currentPage > NOPages)
	{
		crafts.push_back(std::vector<std::unique_ptr<CraftBox>>());
	}
	if (crafts.empty() == true) //after clear empty check
	{
		crafts.push_back(std::vector<std::unique_ptr<CraftBox>>());
	}

	int NOboxes = crafts.at(currentPage).size();
	NOPages = currentPage;

	int verticalSpace = box.getSize().y;
	int heightOfBox = verticalSpace / 7;
	int widthSize = box.getSize().x / 2;
	sf::Vector2f pos = { box.getPosition().x, box.getPosition().y + (heightOfBox * NOboxes) };

	pos.x = PiToPeX(pos.x, res.x);
	pos.y = PiToPeY(pos.y, res.y);
	heightOfBox = PiToPeY(heightOfBox, res.y);
	widthSize = PiToPeX(widthSize, res.x);

	crafts.at(currentPage).push_back(std::make_unique<CraftBox>(pos, sf::Vector2f(widthSize, heightOfBox), TexForBoxes, texCoords, NOitemsNeeded, ingredientsID, resultID, font, res));

	pageIndicator.setString(std::to_string(activePage) + "/" + std::to_string(NOPages));
}

int GUI::CraftingInferface::CheckInput()
{

	for (unsigned int i = 0; i < crafts.size(); i++)
	{
		for (unsigned int j = 0; j < crafts.at(i).size(); j++)
		{
			if (crafts.at(i).at(j)->GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
			{
				needsRefresh = true;
				selectedIngredients.clear();
				selectedAmounts.clear();
				selectedIngredients = crafts.at(i).at(j)->GetIngredientsID();
				selectedAmounts = crafts.at(i).at(j)->GetAmounts();
				selectedResult = crafts.at(i).at(j)->GetResultID();
				return (crafts.at(i).size()) * i + j;
			}
		}
	}
	return -1;
}

GUI::CraftBox& GUI::CraftingInferface::GetCraftBoxAt(int index)
{
	int page = index / 6;
	int pos = index - (page * 6);
	std::cout << "Returning craftbox at page " << page << " and position " << pos << std::endl;
	return *crafts.at(page).at(pos);
}

void GUI::CraftingInferface::Update(sf::Vector2i mousePosWindow, float dt)
{
	UpdateKeyTimer(dt);
	descBox.Update(mousePosWindow, dt);
	if (descBox.GetButton(0).GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
	{
		descBox.GetButton(0).state = btnStates::BTN_IDLE;
		setShown(false);
		descBox.Show(false);
		nextPage.SetShown(false);
		previousPage.SetShown(false);
	}
	if (crafts.empty() == false)
	{

		for (auto& a : crafts.at(activePage))
		{
			a->Update(mousePosWindow);
		}
	}

	nextPage.Update(mousePosWindow);
	previousPage.Update(mousePosWindow);

	if (nextPage.GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
	{
		activePage++;
		if (activePage > NOPages)
		{
			activePage = 0;
		}
		pageIndicator.setString(std::to_string(activePage) + "/" + std::to_string(NOPages));
	}

	if (previousPage.GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
	{
		activePage--;
		if (activePage < 0)
		{
			activePage = NOPages;
		}
		pageIndicator.setString(std::to_string(activePage) + "/" + std::to_string(NOPages));
	}
}

void GUI::CraftingInferface::AfterInit(sf::Vector2f res)
{
	this->res = res;

	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	this->box.setSize(size);
	this->box.setPosition(position);

	this->descBox.AfterInit(res);
}

void GUI::CraftingInferface::Display(sf::RenderTarget& trg)
{

	if (shown == true)
	{
		trg.draw(box);
		if (crafts.empty() == false)
		{

			for (auto& a : crafts.at(activePage))
			{
				a->Display(trg);
			}
		}
		descBox.Display(trg);
		nextPage.Display(trg);
		previousPage.Display(trg);
		trg.draw(pageIndicator);
	}
}

void GUI::CraftingInferface::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}
#include "GUI/GUI.hpp"

//constructors / destructors
GUI::DropdownList::DropdownList(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
	sf::Font& font, std::vector<std::string> texts, sf::Vector2f res) :
	font(font)
{
	keyTime = 0.f;
	keyTimeMax = 10.f;
	isShown = false;
	posPerc = { posXPerc, posYPerc };
	this->sizePerc = sizePerc;
	InitDropdownList(posXPerc, posYPerc, sizePerc, font, texts, res);
}

GUI::DropdownList::~DropdownList()
{
}

//accessors

bool GUI::DropdownList::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

const std::string GUI::DropdownList::GetActiveText()
{
	return this->activeEl->GetText();
}

//modifiers

void GUI::DropdownList::SetPosition(sf::Vector2f pos)
{
	this->activeEl->SetPosition(pos);

	float offsetY = activeEl->GetPos().y;

	for (auto& a : list)
	{
		offsetY += activeEl->GetSize().y;
		a->SetPosition(activeEl->GetPos().x, offsetY);
	}
}

void GUI::DropdownList::SetSize(float x, float y)
{
	this->activeEl->SetSize(x, y);

	for (auto& a : list)
	{
		a->SetSize(x, y);
	}
}

void GUI::DropdownList::SetCharSize(unsigned int size)
{
	this->activeEl->SetCharSize(size);

	for (auto& a : list)
	{
		a->SetCharSize(size);
	}
}

/*******************public functions***********************************/

void GUI::DropdownList::AferInit(sf::Vector2f res)
{
	SetCharSize(CaclCharSize(res));
	SetSize(PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y));
	SetPosition(sf::Vector2f(PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y)));
}

void GUI::DropdownList::Update(float dt, const sf::Vector2i& mousePosWindow)
{

	UpdateKeyTimer(dt);
	UpdateButtons(mousePosWindow);

	//if clicked activeEl, show drplist
	if (activeEl->GetState() == btnStates::BTN_PRESSED && GetKeyTimer())
	{
		isShown = !isShown;
	}
}

void GUI::DropdownList::Display(sf::RenderTarget& target)
{
	//if drp list opened, dsiplay it else only active el
	if (isShown == true)
	{
		for (auto& a : list)
		{
			a->Display(target);
		}
	}
	activeEl->Display(target);
}

//private functions

void GUI::DropdownList::InitDropdownList(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
	sf::Font& font, std::vector<std::string> texts, sf::Vector2f res)
{

	activeEl = std::unique_ptr<GUI::Button>(new GUI::Button(posXPerc, posYPerc, sizePerc, font, texts.at(0), btnTypes::basic, res));

	float offsetY = posYPerc;

	for (unsigned int i = 0; i < texts.size(); i++)
	{ //starts on second element because first is the activeEl as default
		offsetY += sizePerc.y - 0.1f;
		list.push_back(std::unique_ptr<GUI::Button>(new GUI::Button(posXPerc, offsetY, sizePerc, font, texts.at(i), btnTypes::basic, res)));
	}
}

void GUI::DropdownList::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

void GUI::DropdownList::UpdateButtons(const sf::Vector2i& mousePosWindow)
{
	activeEl->Update(mousePosWindow);

	if (isShown == true)
	{
		for (auto& a : list)
		{
			a->Update(mousePosWindow); //updates buttons
		}

		for (auto& a : list) //checks if any was pressed and switches them
		{
			if (a->GetState() == btnStates::BTN_PRESSED && GetKeyTimer() == true)
			{ //when btn from drp menu clicked, activeEL changes text to that of bnt clicked
				activeEl->SetText(a->GetText());
				isShown = false;
			}
		}
	}
}
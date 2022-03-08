#include "PauseMenu.hpp"

//constructor / destructor
PauseMenu::PauseMenu(sf::Vector2f res)
{
	this->currentRes = res;
	std::cout << "Current res:" << res.x << ", " << res.y << std::endl;

	bgr.setSize(res);
	bgr.setFillColor(sf::Color(82, 84, 88, 88));

	container.setSize(sf::Vector2f(res.x / 3.f, res.y));
	container.setFillColor(sf::Color(64, 64, 80, 100));
	container.setPosition(sf::Vector2f((res.x / 3.f), 0.f));
}

PauseMenu::~PauseMenu()
{
}

//accessors

GUI::Button* PauseMenu::GetButton(std::string&& key)
{

	return btns.at(key).get();
}

GUI::DropdownList& PauseMenu::GetDropDown(std::string&& key)
{
	return *dropdowns.at(key);
}
//modifiers

void PauseMenu::SetCharSize(const std::string& key, unsigned int size)
{
	btns.at(key)->SetCharSize(size);
}

void PauseMenu::SetSize(const std::string& key, float x, float y)
{
	btns.at(key)->SetSize(x, y);
}

void PauseMenu::SetPosition(const std::string& key, float y)
{
	btns.at(key)->SetPosition(container.getPosition().x + (container.getSize().x / 2.f) - (btns.at(key)->GetSize().x / 2.f), y);
}

void PauseMenu::SetSizeForBgr(sf::RenderWindow& win)
{
	bgr.setSize(static_cast<sf::Vector2f>(win.getSize()));

	container.setSize(sf::Vector2f(win.getSize().x / 3.f, win.getSize().y));
	container.setPosition(sf::Vector2f((win.getSize().x / 3.f), 0.f));
}

//public functions

void PauseMenu::InitButtons()
{
}

void PauseMenu::AfterInit(sf::Vector2f res)
{
	this->currentRes = res;

	bgr.setSize(res);

	container.setSize(sf::Vector2f(res.x / 3.f, res.y));
	container.setPosition(sf::Vector2f((res.x / 3.f), 0.f));

	for (auto& a : btns)
	{
		a.second->AferInit(res);
	}

	for (auto& a : dropdowns)
	{
		a.second->AferInit(res);
	}
}

void PauseMenu::Update(const sf::Vector2i& mousePosWindow, float dt)
{
	for (auto& b : btns)
	{
		b.second->Update(mousePosWindow);
	}

	for (auto& b : dropdowns)
	{
		b.second->Update(dt, mousePosWindow);
	}
}

void PauseMenu::Display(sf::RenderTarget& target)
{
	target.draw(bgr);
	target.draw(container);
	for (auto& b : btns)
	{
		b.second->Display(target);
	}

	for (auto& c : dropdowns)
	{
		c.second->Display(target);
	}
}

//private functions
void PauseMenu::AddButton(const std::string& key, const std::string& text, float posYPerc, sf::Vector2f sizePerc, sf::Font& font)
{

	sf::Vector2f size = { GUI::PeToPiX(sizePerc.x, currentRes.x), GUI::PeToPiY(sizePerc.y, currentRes.y) };

	float posXPerc = container.getPosition().x + (container.getSize().x / 2.f) - (size.x / 2.f); //classic value, not percentage yet
	posXPerc = (posXPerc / currentRes.x) * 100;													 //conversion from normal value to percentage value

	btns[key] = std::unique_ptr<GUI::Button>(new GUI::Button(posXPerc, posYPerc, sf::Vector2f(sizePerc.x, sizePerc.y), font, text, GUI::btnTypes::basic, currentRes));
}

void PauseMenu::AddDropDown(const std::string& key, float posYPerc, sf::Vector2f sizePerc, sf::Font& font, std::vector<std::string>& texts)
{

	sf::Vector2f size = { GUI::PeToPiX(sizePerc.x, currentRes.x), GUI::PeToPiY(sizePerc.y, currentRes.y) };

	float posXPerc = container.getPosition().x + (container.getSize().x / 2.f) - (size.x / 2.f); //classic value, not percentage yet
	posXPerc = (posXPerc / currentRes.x) * 100;													 //conversion from normal value to percentage value

	dropdowns[key] = std::unique_ptr<GUI::DropdownList>(new GUI::DropdownList(posXPerc, posYPerc, sizePerc, font, texts, currentRes));
}

#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "GUI/GUI.hpp"
#include <map>

class PauseMenu
{
	//variables
public:
	sf::RectangleShape container;
	sf::RectangleShape bgr;
	sf::Vector2f currentRes;

	std::map<std::string, std::unique_ptr<GUI::Button>> btns;
	std::map<std::string, std::unique_ptr<GUI::DropdownList>> dropdowns;
	//constructors
public:
	PauseMenu(sf::Vector2f res);

	~PauseMenu();

	// accessors
public:
	//returns button at key
	GUI::Button* GetButton(std::string&& key);

	GUI::DropdownList& GetDropDown(std::string&& key);

	//modifiers
public:
	//sets char size of button
	void SetCharSize(const std::string& key, unsigned int size);

	//sets size of btn
	void SetSize(const std::string& key, float x, float y);

	//sets position of btn
	void SetPosition(const std::string& key, float y);

	//recalculates sizes for bgr and container
	void SetSizeForBgr(sf::RenderWindow& win);

	//functions
public:
	//initializes all buttons
	void InitButtons();

	//reinits gui after res has been changed
	void AfterInit(sf::Vector2f res);

	//updates pause menu
	void Update(const sf::Vector2i& mousePosWindow, float dt);

	//renders stuff to target
	void Display(sf::RenderTarget& target);

	//adds button
	void AddButton(const std::string& key, const std::string& text, float posYPerc, sf::Vector2f sizePerc, sf::Font& font);

	void AddDropDown(const std::string& key, float posYPerc, sf::Vector2f sizePerc, sf::Font& font, std::vector<std::string>& texts);
};

#endif

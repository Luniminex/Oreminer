#ifndef LEVELCOMPONENT_HPP
#define LEVELCOMPONENT_HPP

#include "AttributeComponent.hpp"
#include "GUI/GUI.hpp"

class LevelComponent
{
	GUI::LevelUpScreen levelScreen;
	AttributeComponent& attrs;

public:
	LevelComponent(AttributeComponent& attrs, sf::Font& font, sf::Vector2f res);
	~LevelComponent();

	void LevelUP();

	void AfterInit(sf::Vector2f res);

	void Update(sf::Vector2i mousePosWin);

	void Display(sf::RenderTarget& trg);
};

#endif
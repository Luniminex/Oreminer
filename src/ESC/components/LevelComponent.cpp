#include "LevelComponent.hpp"

LevelComponent::LevelComponent(AttributeComponent& attrs, sf::Font& font, sf::Vector2f res) :
	levelScreen(sf::Vector2f(25, 5), sf::Vector2f(40, 50), font, res),
	attrs(attrs)
{
}

LevelComponent::~LevelComponent()
{
}

void LevelComponent::LevelUP()
{
	levelScreen.SetShown(true);
	levelScreen.LevelUp(attrs.getAttributes().atrTypes);
}

void LevelComponent::AfterInit(sf::Vector2f res)
{
	levelScreen.AfterInit(res);
}

void LevelComponent::Update(sf::Vector2i mousePosWin)
{
	levelScreen.Update(mousePosWin);
	if (levelScreen.DidLevelUp() == true)
	{
		attrs.getAttributes().atrTypes.at(levelScreen.LeveledID().first) += levelScreen.LeveledID().second;
		std::cout << "Leved thing to: " << std::to_string(attrs.getAttributes().atrTypes.at(levelScreen.LeveledID().first)) << std::endl;
		levelScreen.SetLeveledUP(false);
		levelScreen.SetShown(false);
		attrs.UpdateAttributeInfo();
	}
}

void LevelComponent::Display(sf::RenderTarget& trg)
{
	levelScreen.Display(trg);
}
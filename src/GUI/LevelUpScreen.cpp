#include "GUI/GUI.hpp"

GUI::LevelUpScreen::LevelUpScreen(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res) :
	font(font)
{
	this->posPerc = posPerc;
	this->sizePerc = sizePerc;
	this->res = res;

	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	box.setSize(size);
	box.setPosition(position);
	box.setFillColor(sf::Color(155, 155, 155));

	congrats.setFont(font);
	congrats.setString("Congratulations! You've leveled up!\n");
	congrats.setCharacterSize(CaclCharSize(res));
	congrats.setPosition(sf::Vector2f(position.x + (size.x / 2) - congrats.getGlobalBounds().width / 2, position.y));

	float mid = (box.getSize().x - congrats.getGlobalBounds().width) / 2;

	attributesText.setFont(font);
	attributesText.setString("");
	attributesText.setCharacterSize(CaclCharSizeSmall(res));
	attributesText.setPosition(sf::Vector2f(congrats.getPosition().x - mid / 2, position.y + congrats.getGlobalBounds().height));

	leveledUp = false;
	levelID = -1;
	shown = false;

	increaseValues[ATRTYPES::maxHealth] = 5;
	increaseValues[ATRTYPES::healthRegen] = 1;
	increaseValues[ATRTYPES::maxStamina] = 5;
	increaseValues[ATRTYPES::staminaRegen] = 2;
	increaseValues[ATRTYPES::strenght] = 1;
	increaseValues[ATRTYPES::armour] = 1;
	increaseValues[ATRTYPES::critChance] = 2;
	increaseValues[ATRTYPES::critDamage] = 10;
	increaseValues[ATRTYPES::dodgeChance] = 2;
	increaseValues[ATRTYPES::maxMana] = 5;
	increaseValues[ATRTYPES::manaRegen] = 2;
}

GUI::LevelUpScreen::~LevelUpScreen()
{
}

bool GUI::LevelUpScreen::IsShown()
{
	return this->shown;
}

bool GUI::LevelUpScreen::DidLevelUp()
{
	return this->leveledUp;
}

void GUI::LevelUpScreen::SetLeveledUP(bool leveled)
{
	this->leveledUp = leveled;
}

std::pair<int, int> GUI::LevelUpScreen::LeveledID()
{
	return std::pair(this->levelID, increaseValues.at(this->levelID));
}

void GUI::LevelUpScreen::SetShown(bool shown)
{
	this->shown = shown;
}
void GUI::LevelUpScreen::LevelUp(std::vector<int>& atrTypes)
{
	levelUpBtns.clear();
	attributesText.setString("");

	congrats.setString("Congratulations! You've leveled up!\n");
	congrats.setCharacterSize(CaclCharSize(res));
	congrats.setPosition(sf::Vector2f(box.getPosition().x + (box.getSize().x / 2) - congrats.getGlobalBounds().width / 2, box.getPosition().y));

	std::vector<int> excludedAttrs = { ATRTYPES::experience,
		ATRTYPES::experienceBar,
		ATRTYPES::level,
		ATRTYPES::miningPower,
		ATRTYPES::health,
		ATRTYPES::mana,
		ATRTYPES::stamina };

	int i = 0;
	bool ok = true;
	for (auto& a : atrTypes)
	{
		ok = true;
		for (auto& b : excludedAttrs)
		{
			if (i == b)
			{
				ok = false;
			}
		}
		if (ok == true)
		{
			attributesText.setString(attributesText.getString() + atrNameMap.at(i) + ": " + std::to_string(a) + "\n\n");
		}
		i++;
	}

	i = 0;
	int j = 0;
	int height = attributesText.getGlobalBounds().height / (atrTypes.size() - excludedAttrs.size()) / 2;
	for (auto& a : atrTypes)
	{
		ok = true;
		for (auto& b : excludedAttrs)
		{
			if (i == b)
			{
				ok = false;
			}
		}
		(void)a;
		if (ok == true)
		{

			levelUpBtns[i] = std::make_unique<GUI::Button>(PiToPeX(attributesText.getPosition().x + attributesText.getGlobalBounds().width, res.x), PiToPeY(attributesText.getPosition().y + (j * height * 2) - 0.5 * height, res.y), sf::Vector2f(sizePerc.x / 5, PiToPeY(2 * height, res.y)), font, "Increase + " + std::to_string(increaseValues.at(i)), sf::Color(74, 74, 67, 128), sf::Color(110, 110, 98, 160), sf::Color(145, 145, 129, 200), res);
			levelUpBtns.at(i)->SetCharSize(CaclCharSizeSmallest(res));
			j++;
		}
		i++;
	}

	box.setSize(sf::Vector2f(PeToPiX(sizePerc.x, res.x), attributesText.getGlobalBounds().height + congrats.getGlobalBounds().height));
}

void GUI::LevelUpScreen::AfterInit(sf::Vector2f res)
{

	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	box.setSize(size);
	box.setPosition(position);

	congrats.setFont(font);
	congrats.setCharacterSize(CaclCharSize(res));
	congrats.setPosition(sf::Vector2f(position.x + (size.x / 2) - congrats.getGlobalBounds().width, position.y));

	attributesText.setFont(font);
	attributesText.setCharacterSize(CaclCharSizeSmall(res));
	attributesText.setPosition(sf::Vector2f(position.x, position.y + congrats.getGlobalBounds().height));

	for (auto& a : levelUpBtns)
	{
		a.second->AferInit(res);
		a.second->SetCharSize(CaclCharSizeSmallest(res));
	}
}

void GUI::LevelUpScreen::Update(sf::Vector2i mousePosWin)
{
	if (shown == true)
	{
		int i = 0;
		for (auto& a : levelUpBtns)
		{
			if (a.second->GetState() == btnStates::BTN_PRESSED)
			{
				leveledUp = true;
				levelID = a.first;
				std::cout << "Level up something" << std::endl;
			}
			i++;
		}

		for (auto& a : levelUpBtns)
		{
			a.second->Update(mousePosWin);
		}
	}
}

void GUI::LevelUpScreen::Display(sf::RenderTarget& trg)
{
	if (shown == true)
	{
		trg.draw(box);
		trg.draw(congrats);
		trg.draw(attributesText);
		for (auto& a : levelUpBtns)
		{
			a.second->Display(trg);
		}
	}
}
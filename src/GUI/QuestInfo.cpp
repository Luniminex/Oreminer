#include "GUI.hpp"

GUI::QuestInfo::QuestInfo(float posXPerc, float posYPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res) :
	font(font)
{
	this->posPerc.x = posXPerc;
	this->posPerc.y = posYPerc;

	this->sizePerc.x = sizePerc.x;
	this->sizePerc.y = sizePerc.y;

	std::cout << "X: " << posPerc.x << " Y: " << posPerc.y << std::endl;

	this->res = res;

	questName.setCharacterSize(GUI::CaclCharSizeSmall(res));
	questContext.setCharacterSize(GUI::CaclCharSizeSmall(res));
	goals.setCharacterSize(GUI::CaclCharSizeSmall(res));
	rewards.setCharacterSize(GUI::CaclCharSizeSmall(res));

	questName.setFont(font);
	questContext.setFont(font);
	goals.setFont(font);
	rewards.setFont(font);
}

GUI::QuestInfo::~QuestInfo()
{
}

void GUI::QuestInfo::SetQuestName(const std::string& text)
{
	questName.setString(text + "\n");
	questName.setString(GUI::FitTextInsideBox(questName, GUI::PeToPiX(sizePerc.x, res.x)));
	questName.setPosition(GUI::PeToPiX(posPerc.x, res.x), GUI::PeToPiY(posPerc.y, res.y));
}

void GUI::QuestInfo::SetQuestContext(const std::string& text)
{
	questContext.setString(text + "\n");
	questContext.setString(GUI::FitTextInsideBox(questContext, GUI::PeToPiX(sizePerc.x, res.x)));
	questContext.setPosition(sf::Vector2f(GUI::PeToPiX(posPerc.x, res.x), questName.getGlobalBounds().height + GUI::PeToPiY(posPerc.y, res.y)));
}

void GUI::QuestInfo::SetGoals(std::vector<std::string>& texts)
{
	std::string temp;
	temp = "";

	for (auto& a : texts)
	{
		temp = temp + a + "\n";
	}

	goals.setString(temp);
	goals.setPosition(questContext.getPosition().x + GUI::PeToPiX(sizePerc.x + 3.f, res.x), GUI::PeToPiY(posPerc.y, res.y));
}

void GUI::QuestInfo::CompleteGoals()
{
	std::string temp = goals.getString();

	//inserts string after every \n in string
	for (size_t i = 1; i < temp.length(); i++)
	{
		if (temp[i] == '\n')
		{
			temp.insert(i, " - Completed");
			i += 12;
		}
	}

	goals.setString(std::string(temp));
	goals.setPosition(questContext.getPosition().x + GUI::PeToPiX(sizePerc.x + 3.f, res.x), GUI::PeToPiY(posPerc.y, res.y));
}

void GUI::QuestInfo::SetRewards(std::vector<std::string>& texts)
{
	std::string temp;
	temp = "";

	for (auto& a : texts)
	{
		temp = temp + a + "\n";
	}

	rewards.setString(temp);
	rewards.setPosition(goals.getPosition().x, goals.getPosition().y + goals.getGlobalBounds().height);
}

void GUI::QuestInfo::AfterInit(sf::Vector2f res)
{

	this->res = res;

	questName.setCharacterSize(GUI::CaclCharSizeSmallest(res));
	questContext.setCharacterSize(GUI::CaclCharSizeSmallest(res));
	goals.setCharacterSize(GUI::CaclCharSizeSmallest(res));
	rewards.setCharacterSize(GUI::CaclCharSizeSmallest(res));

	questName.setPosition(GUI::PeToPiX(posPerc.x, res.x), GUI::PeToPiY(posPerc.y, res.y));
	questContext.setPosition(sf::Vector2f(GUI::PeToPiX(posPerc.x, res.x), questName.getGlobalBounds().height + GUI::PeToPiY(posPerc.y, res.y)));
	goals.setPosition(questContext.getPosition().x + GUI::PeToPiX(sizePerc.x + 3.f, res.x), GUI::PeToPiY(posPerc.y, res.y));
	rewards.setPosition(goals.getPosition().x, goals.getPosition().y + goals.getGlobalBounds().height);

	questName.setString(GUI::FitTextInsideBox(questName, GUI::PeToPiX(sizePerc.x, res.x)));
	questContext.setString(GUI::FitTextInsideBox(questContext, GUI::PeToPiX(sizePerc.x, res.x)));
}

void GUI::QuestInfo::Update()
{
	//I am lazy so you do that
}

void GUI::QuestInfo::Display(sf::RenderTarget& trg)
{
	trg.draw(questName);
	trg.draw(questContext);
	trg.draw(goals);
	trg.draw(rewards);
}
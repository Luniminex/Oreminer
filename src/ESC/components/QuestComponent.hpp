#ifndef QUESTCOMPONENT_HPP
#define QUESTCOMPONENT_HPP

#include "GUI/GUI.hpp"
#include "containers/Quest.hpp"

class Quest;

class QuestComponent
{

private:
	GUI::QuestInfo questInfo;
	Quest* activeQuest;
	bool allQuestsCompleted;
	bool hasActiveQuest;
	bool readyToRecieveQuest;

public:
	QuestComponent(sf::Vector2f res, sf::Font& font);
	~QuestComponent();

public:
	bool AreGoalsCompleted();

	bool IsActiveQuest();

	bool IsReadyToQuest();

	Quest& GetActiveQuest();

public:
	void SetReadyToQuest(bool ready);

	void SetQuest(Quest& quest, std::vector<std::string>& goalName, std::vector<std::string>& rewardName);

	void Complete();

	void UpdateQuest(int id, int amount);

public:
	void SetAllQuestCompleted(bool completed);

	void AfterInit(sf::Vector2f res);

	void Update();

	void Display(sf::RenderTarget& trg);
};

#endif
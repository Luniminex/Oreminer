#include "QuestComponent.hpp"

QuestComponent::QuestComponent(sf::Vector2f res, sf::Font& font) :
	questInfo(0.f, 0.f, sf::Vector2f(13.f, 15.f), font, res)
{

	allQuestsCompleted = false;
	hasActiveQuest = false;
	readyToRecieveQuest = true;

	activeQuest = nullptr;

	questInfo.SetQuestName("");
	questInfo.SetQuestContext("");

	std::vector<std::string> temp;
	temp.push_back("");
	temp.push_back("");

	questInfo.SetGoals(temp);
	temp.clear();
	temp.push_back("");
	temp.push_back("");
	questInfo.SetRewards(temp);
	temp.clear();
}

QuestComponent::~QuestComponent()
{
	activeQuest = NULL;
}

bool QuestComponent::AreGoalsCompleted()
{
	return allQuestsCompleted;
}

bool QuestComponent::IsActiveQuest()
{
	return hasActiveQuest;
}

Quest& QuestComponent::GetActiveQuest()
{
	return *activeQuest;
}

bool QuestComponent::IsReadyToQuest()
{
	return readyToRecieveQuest;
}

/********public functions**********/

void QuestComponent::SetQuest(Quest& quest, std::vector<std::string>& goalName, std::vector<std::string>& rewardName)
{
	hasActiveQuest = true;
	activeQuest = &quest;
	questInfo.SetQuestName(quest.name);
	questInfo.SetQuestContext(quest.context);
	questInfo.SetGoals(goalName);
	questInfo.SetRewards(rewardName);
}

void QuestComponent::Complete()
{
	hasActiveQuest = false;
	questInfo.CompleteGoals();
}

void QuestComponent::SetReadyToQuest(bool ready)
{
	readyToRecieveQuest = ready;
}

void QuestComponent::UpdateQuest(int id, int amount)
{
	if (activeQuest != nullptr)
	{
		for (unsigned int i = 0; i < activeQuest->GoalID.size(); i++)
		{
			if (activeQuest->GoalID.at(i) == id)
			{
				if (activeQuest->GoalDone.at(i) < activeQuest->GoalAmounts.at(i))
				{
					activeQuest->GoalDone.at(i) += amount;
				}
				else
				{
					std::cout << "Goal done" << std::endl;
				}
			}
		}
	}
}

void QuestComponent::SetAllQuestCompleted(bool completed)
{
	allQuestsCompleted = completed;
}

void QuestComponent::AfterInit(sf::Vector2f res)
{
	questInfo.AfterInit(res);
}

void QuestComponent::Update()
{
	questInfo.Update();
}

void QuestComponent::Display(sf::RenderTarget& trg)
{
	questInfo.Display(trg);
}
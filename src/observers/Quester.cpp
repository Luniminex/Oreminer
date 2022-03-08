#include "Quester.hpp"

ResourceManager* Quester::resourceman = nullptr;
Player* Quester::player = nullptr;
std::vector<Quest> Quester::quests;
std::map<int, std::string> Quester::enemies;
std::map<int, std::string> Quester::npcs;
//std::map<int, std::string> Quester::items;
std::map<int, std::unique_ptr<Item>>* Quester::items;

Quester::Quester() :
	IObserver()
{
}

Quester::~Quester()
{
}

void Quester::Process(const int message, int id, int amount = 1)
{
	switch (message)
	{
		case SubjectActions::KILL_ENEMY:
			AnswerKillEnemy(id, amount);
			break;
		case SubjectActions::GIVE_QUEST:
			AnswerGiveQuest(id);
			break;
		case SubjectActions::TALK_TO_NPC:
			AnswerTalkToNPC(id);
			break;
		case SubjectActions::GATHER_ITEM:
			AnswerGatherItem(id, amount);
			break;
		case SubjectActions::COMPLETE_QUEST:
			AnswerCompleteQuest(id);
			break;
		default:
			break;
	};
}

//private functions

void Quester::LoadQuests(int save)
{
	quests.clear();
	Quester::resourceman = ResourceManager::GetInstance();

	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(resourceman->SaveDataAt(save, "QUESTS"));
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD QUEST DATA" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("quests")->first_node("quest");

	const char* temp = nullptr;

	int questid = 0;
	int state = 0;
	int unlockCondition = 0;
	int type = 0;

	std::vector<int> goalID;
	std::vector<int> goalAmounts;
	std::vector<int> goalDone;

	std::vector<int> rewardID;
	std::vector<int> rewardAmount;

	std::string name = "";
	std::string context = "";

	enum strings
	{
		Equestid,
		Estate,
		Eunlockcondition,
		Etype,
		Egoal,
		Ereward,
		Egoalid,
		Egoalamount,
		Egoaldone,
		Erewardid,
		Erewardamount,
		Ename,
		Econtext
	};

	std::map<std::string, int> strMap; //map to use in switch statement

	strMap["id"] = strings::Equestid;
	strMap["state"] = strings::Estate;
	strMap["unlockcondition"] = strings::Eunlockcondition;
	strMap["name"] = strings::Ename;
	strMap["context"] = strings::Econtext;
	strMap["type"] = strings::Etype;
	strMap["elementid"] = strings::Egoalid;
	strMap["number"] = strings::Egoalamount;
	strMap["done"] = strings::Egoaldone;
	strMap["rewardid"] = strings::Erewardid;
	strMap["amount"] = strings::Erewardamount;
	strMap["goal"] = strings::Egoal;
	strMap["reward"] = strings::Ereward;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			if (std::string(parent->name()) == "quest")
			{
				switch (strMap.at(child->name()))
				{
					case strings::Equestid:
						temp = child->value();
						questid = std::stoi(temp);
						break;
					case strings::Estate:
						temp = child->value();
						state = std::stoi(temp);
						break;
					case strings::Eunlockcondition:
						temp = child->value();
						unlockCondition = std::stoi(temp);
						break;
					case strings::Ename:
						name = child->value();
						break;
					case strings::Econtext:
						context = child->value();
						break;
					case strings::Etype:
						temp = child->value();
						type = std::stoi(temp);
						break;
					case strings::Egoal:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::Egoalid:
									temp = grandchild->value();
									goalID.push_back(std::stoi(temp));
									break;
								case strings::Egoalamount:
									temp = grandchild->value();
									goalAmounts.push_back(std::stoi(temp));
									break;
								case strings::Egoaldone:
									temp = grandchild->value();
									goalDone.push_back(std::stoi(temp));
									break;
								default:
									break;
							}
						}
						break;
					case strings::Ereward:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::Erewardid:
									temp = grandchild->value();
									rewardID.push_back(std::stoi(temp));
									break;
								case strings::Erewardamount:
									temp = grandchild->value();
									rewardAmount.push_back(std::stoi(temp));
									break;
								default:
									break;
							}
						}
						break;
					default:
						break;
				}
			}
		}

		quests.push_back(Quest(questid, state, unlockCondition, type, goalID, goalAmounts, goalDone, rewardID, rewardAmount, name, context));

		goalID.clear();
		goalAmounts.clear();
		rewardID.clear();
		rewardAmount.clear();
	}

	doc.clear();
	delete temp;
	/*************xml****************/
}

void Quester::LoadPlayer(Player* playerptr)
{
	player = playerptr;
}

void Quester::LoadEnemy(int id, const std::string& enemyName)
{
	enemies[id] = enemyName;
}

void Quester::LoadNPC(int id, const std::string npcName)
{
	npcs[id] = npcName;
}

void Quester::LoadItem(std::map<int, std::unique_ptr<Item>>* itemTemplates)
{
	items = itemTemplates;
}

void Quester::ClearAll()
{

	quests.clear();
	enemies.clear();
	items = NULL;
	player = NULL;
}

void Quester::AnswerKillEnemy(int id, int amount)
{
	bool changeMade = false;
	std::pair<bool, int> allGoalsCompleted(false, -1);
	for (auto& a : quests)
	{
		if (a.state == QuestStates::inProgress && a.type == QuestTypes::KILL)
		{
			changeMade = false;
			std::vector<bool> goalsDone;
			for (auto& b : a.GoalID)
			{
				(void)b;
				goalsDone.push_back(false);
			}
			for (unsigned int i = 0; i < a.GoalID.size(); i++)
			{
				if (a.GoalID.at(i) == id)
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						a.GoalDone.at(i) += amount;
						goalsDone.at(i) = false;
						changeMade = true;
						if (a.GoalDone.at(i) >= a.GoalAmounts.at(i))
						{
							goalsDone.at(i) = true;
						}
					}
					else
					{

					}
				}
				else
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						goalsDone.at(i) = false;
					}
					else
					{
						goalsDone.at(i) = true;
					}
				}
			}
			bool allDone = true;
			for (auto b : goalsDone)
			{
				if (b == false)
				{
					allDone = false;
				}
			}
			if (allDone == true)
			{
				allGoalsCompleted.first = true;
				allGoalsCompleted.second = a.id;
			}
			else
			{
			}
		}
	}

	if (changeMade)
	{
		std::vector<std::string> goals;
		std::vector<std::string> rewards;
		for (auto& a : quests)
		{
			if (a.state == QuestStates::inProgress && a.type == SubjectActions::KILL_ENEMY)
			{
				for (unsigned int i = 0; i < a.GoalID.size(); i++)
				{
					goals.push_back("Killed " + std::to_string(a.GoalDone.at(i)) + "/" + std::to_string(a.GoalAmounts.at(i)) + " " + enemies.at(a.GoalID.at(i)));
				}
				//sets the quest
				for (unsigned int i = 0; i < a.rewardID.size(); i++)
				{
					rewards.push_back("Reward: " + items->at(a.rewardID.at(i))->GetName() + " " + std::to_string(a.rewardAmount.at(i)));
				}
				player->SetQuest(a, goals, rewards);
			}
		}
	}
	if (allGoalsCompleted.first == true)
	{
		player->CompleteGoals();
		player->GetQuestComponent().SetAllQuestCompleted(true);
	}
}

void Quester::AnswerGatherItem(int id, int amount)
{
	bool changeMade = false;
	std::pair<bool, int> allGoalsCompleted(false, -1);
	for (auto& a : quests)
	{
		if (a.state == QuestStates::inProgress && a.type == QuestTypes::GATHER)
		{
			changeMade = false;
			std::vector<bool> goalsDone;
			for (auto& b : a.GoalID)
			{
				(void)b;
				goalsDone.push_back(false);
			}
			for (unsigned int i = 0; i < a.GoalID.size(); i++)
			{
				if (a.GoalID.at(i) == id)
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						a.GoalDone.at(i) += amount;
						goalsDone.at(i) = false;
						changeMade = true;
						if (a.GoalDone.at(i) >= a.GoalAmounts.at(i))
						{
							goalsDone.at(i) = true;
						}
					}
					else
					{

					}
				}
				else
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						goalsDone.at(i) = false;
					}
					else
					{
						goalsDone.at(i) = true;
					}
				}
			}
			bool allDone = true;
			for (auto b : goalsDone)
			{
				if (b == false)
				{
					allDone = false;
				}
			}
			if (allDone == true)
			{
				allGoalsCompleted.first = true;
				allGoalsCompleted.second = a.id;
			}
			else
			{
			}
		}
	}

	if (changeMade)
	{
		std::vector<std::string> goals;
		std::vector<std::string> rewards;
		for (auto& a : quests)
		{
			if (a.state == QuestStates::inProgress && a.type == QuestTypes::GATHER)
			{
				for (unsigned int i = 0; i < a.GoalID.size(); i++)
				{
					goals.push_back("Gathered " + std::to_string(a.GoalDone.at(i)) + "/" + std::to_string(a.GoalAmounts.at(i)) + " " + items->at(a.GoalID.at(i))->GetName());
				}
				//sets the quest
				for (unsigned int i = 0; i < a.rewardID.size(); i++)
				{
					rewards.push_back("Reward: " + items->at(a.rewardID.at(i))->GetName() + " " + std::to_string(a.rewardAmount.at(i)));
				}
				player->SetQuest(a, goals, rewards);
			}
		}
	}
	if (allGoalsCompleted.first == true)
	{
		player->CompleteGoals();
		player->GetQuestComponent().SetAllQuestCompleted(true);
	}
}
void Quester::AnswerTalkToNPC(int id)
{
	bool changeMade = false;
	std::pair<bool, int> allGoalsCompleted(false, -1);
	for (auto& a : quests)
	{
		if (a.state == QuestStates::inProgress && a.type == QuestTypes::TALK)
		{
			changeMade = false;
			std::vector<bool> goalsDone;
			for (auto& b : a.GoalID)
			{
				(void)b;
				goalsDone.push_back(false);
			}
			for (unsigned int i = 0; i < a.GoalID.size(); i++)
			{
				if (a.GoalID.at(i) == id)
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						a.GoalDone.at(i) += 1;
						goalsDone.at(i) = false;
						changeMade = true;
						if (a.GoalDone.at(i) >= a.GoalAmounts.at(i))
						{
							goalsDone.at(i) = true;
						}
					}
					else
					{

					}
				}
				else
				{
					if (a.GoalDone.at(i) < a.GoalAmounts.at(i))
					{
						goalsDone.at(i) = false;
					}
					else
					{
						goalsDone.at(i) = true;
					}
				}
			}
			bool allDone = true;
			for (auto b : goalsDone)
			{
				if (b == false)
				{
					allDone = false;
				}
			}
			if (allDone == true)
			{
				allGoalsCompleted.first = true;
				allGoalsCompleted.second = a.id;
			}
			else
			{
			}
		}
	}

	if (changeMade)
	{
		std::vector<std::string> goals;
		std::vector<std::string> rewards;
		for (auto& a : quests)
		{
			if (a.state == QuestStates::inProgress && a.type == QuestTypes::GATHER)
			{
				for (unsigned int i = 0; i < a.GoalID.size(); i++)
				{
					goals.push_back("Talked to " + npcs.at(id));
				}
				//sets the quest
				for (unsigned int i = 0; i < a.rewardID.size(); i++)
				{
					rewards.push_back("Reward: " + items->at(a.rewardID.at(i))->GetName() + " " + std::to_string(a.rewardAmount.at(i)));
				}
				player->SetQuest(a, goals, rewards);
			}
		}
	}
	if (allGoalsCompleted.first == true)
	{
		player->CompleteGoals();
		player->GetQuestComponent().SetAllQuestCompleted(true);
	}
}

void Quester::AnswerGiveQuest(int id)
{
	if (player == nullptr)
	{
		return;
	}
	for (auto& a : quests)
	{
		if (a.id == id && a.state == QuestStates::unlocked)
		{
			player->GetQuestComponent().SetAllQuestCompleted(false);
			player->GetQuestComponent().SetReadyToQuest(false);

			a.state = QuestStates::inProgress;
			std::vector<std::string> goals;
			std::vector<std::string> rewards;

			switch (a.type)
			{
				case QuestTypes::KILL:
					for (unsigned int i = 0; i < a.GoalID.size(); i++)
					{
						goals.push_back("Killed " + std::to_string(a.GoalDone.at(i)) + "/" + std::to_string(a.GoalAmounts.at(i)) + " " + enemies.at(a.GoalID.at(i)));
					}
					break;
				case QuestTypes::TALK:
					for (unsigned int i = 0; i < a.GoalID.size(); i++)
					{
						goals.push_back("Talk to " + npcs.at(a.GoalID.at(i)));
					}
					break;
				case QuestTypes::GATHER:
					for (unsigned int i = 0; i < a.GoalID.size(); i++)
					{
						goals.push_back("Gathered " + std::to_string(a.GoalDone.at(i)) + "/" + std::to_string(a.GoalAmounts.at(i)) + " " + items->at(a.GoalID.at(i))->GetName());
					}
					break;
				default:
					break;
			}

			//loads rewards
			for (unsigned int i = 0; i < a.rewardID.size(); i++)
			{
				rewards.push_back("Reward: " + items->at(a.rewardID.at(i))->GetName() + " " + std::to_string(a.rewardAmount.at(i)));
			}
			//sets the quest
			player->SetQuest(a, goals, rewards);
		}
	}
}

void Quester::AnswerCompleteQuest(int id)
{

	for (size_t i = 0; i < quests.size(); i++)
	{
		if (quests.at(i).id == id && quests.at(i).state == QuestStates::inProgress)
		{
			if (quests.at(i).type == QuestTypes::GATHER)
			{
				for (unsigned int j = 0; j < quests.at(i).GoalAmounts.size(); j++)
				{
					player->GetInventoryComponent().RemoveItemByID(quests.at(i).GoalID.at(j), quests.at(i).GoalAmounts.at(j));
				}
			}

			for (unsigned int j = 0; j < quests.at(i).rewardID.size(); j++)
			{
				player->GiveItem(*items->at(quests.at(i).rewardID.at(j)), quests.at(i).rewardAmount.at(j));
			}
			quests.at(i).state = QuestStates::done;
			player->GetQuestComponent().SetReadyToQuest(true);
			if (i < quests.size() - 1)
			{
				quests.at(i + 1).state = QuestStates::unlocked;
			}
		}
	}
}
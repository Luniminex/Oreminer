#include "Statistics.hpp"

StatisticsContainer Statistics::statCont;
ResourceManager* Statistics::resourceman = nullptr;

Statistics::Statistics() :
	IObserver()
{
}

Statistics::~Statistics()
{
}

StatisticsContainer& Statistics::GetStatisticsContainer()
{
	return statCont;
}

void Statistics::Process(const int message, int id, int amount)
{
	std::cout << "STATISTICS::Notyfing statistics" << std::endl;

	switch (message)
	{

		case SubjectActions::KILL_ENEMY:
			statCont.uniqueEnemiesKilled.at(id) += amount;
			break;
		case SubjectActions::TALK_TO_NPC:
			statCont.npcsSeen.at(id) = true;
			break;
		case SubjectActions::COMPLETE_QUEST:
			statCont.questCompleted.at(id) = true;
			break;
		default:
			break;
	}
}

void Statistics::LoadStatistics(int save)
{

	statCont.enemiesKilled = 0;
	statCont.npcsSeen.clear();
	statCont.questCompleted.clear();
	statCont.uniqueEnemiesKilled.clear();

	Statistics::resourceman = ResourceManager::GetInstance();

	std::cout << "loading statistics data" << std::endl;
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(resourceman->SaveDataAt(save, "PLAYERSTATISTICS"));
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD STATISTICS DATA" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("statistics")->first_node("statistic");

	const char* temp = nullptr;

	int allEnemiesKilled = 0;

	std::map<int, int> enemiesKilled;
	std::map<int, bool> npcSeen;
	std::map<int, bool> questsCompleted;

	int enemyid = 0;
	int npcid = 0;
	int questid = 0;

	bool questcompleted = false;
	bool npcseen = false;
	int enemieskiled = 0;

	enum strings
	{
		EallEnemiesKilled,
		EuniqueEnemiesKilled,
		EnpcSeen,
		EquestCompleted,
		EenemyID,
		EnpcID,
		EquestID,
		EenemyKills,
		EisQuestCompleted,
		EuniqueEnemyKilled,
		EuniqueNPCSeen,
		EuniqueQuestCompleted
	};

	std::map<std::string, int> strMap; //map to use in switch statement

	strMap["enemieskilled"] = strings::EallEnemiesKilled;
	strMap["enemyid"] = strings::EenemyID;
	strMap["npcid"] = strings::EnpcID;
	strMap["questid"] = strings::EquestID;
	strMap["count"] = strings::EuniqueEnemyKilled;
	strMap["seen"] = strings::EnpcSeen;
	strMap["completed"] = strings::EquestCompleted;
	strMap["uniqueenemykilled"] = strings::EuniqueEnemiesKilled;
	strMap["npcseen"] = strings::EuniqueNPCSeen;
	strMap["questcompleted"] = strings::EuniqueQuestCompleted;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			if (std::string(parent->name()) == "statistic")
			{
				switch (strMap.at(child->name()))
				{
					case strings::EallEnemiesKilled:
						temp = child->value();
						allEnemiesKilled = std::stoi(temp);
						break;
					case strings::EuniqueEnemiesKilled:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::EenemyID:
									temp = grandchild->value();
									enemyid = std::stoi(temp);
									break;
								case strings::EuniqueEnemyKilled:
									temp = grandchild->value();
									enemieskiled = std::stoi(temp);
									break;
								default:
									break;
							}
						}
						enemiesKilled[enemyid] = enemieskiled;
						break;
					case strings::EuniqueNPCSeen:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::EnpcID:
									temp = grandchild->value();
									npcid = std::stoi(temp);
									break;
								case strings::EnpcSeen:
									temp = grandchild->value();
									npcseen = std::stoi(temp);
									break;
								default:
									break;
							}
						}
						npcSeen[npcid] = npcseen;
						break;
					case strings::EuniqueQuestCompleted:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::EquestID:
									temp = grandchild->value();
									questid = std::stoi(temp);
									break;
								case strings::EquestCompleted:
									temp = grandchild->value();
									questcompleted = std::stoi(temp);
									break;
								default:
									break;
							}
						}
						questsCompleted[questid] = questcompleted;
						break;
					default:
						break;
				}
			}
		}

		statCont.enemiesKilled = allEnemiesKilled;
		statCont.uniqueEnemiesKilled = enemiesKilled;
		statCont.npcsSeen = npcSeen;
		statCont.questCompleted = questsCompleted;
	}

	doc.clear();
	delete temp;
	std::cout << "Loading statistics data succesfull" << std::endl;
	/*************xml****************/
}

void Statistics::ClearAll()
{
	statCont.enemiesKilled = 0;
	statCont.npcsSeen.clear();
	statCont.questCompleted.clear();
	statCont.uniqueEnemiesKilled.clear();
}
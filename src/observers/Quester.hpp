#ifndef QUESTER_HPP
#define QUESTER_HPP

#include "ESC/entities/Player.hpp"
#include "containers/Quest.hpp"
#include "managers/ResourceManager.hpp"
#include "observers/Observer.hpp"

#include <rapidxml-1.13/rapidxml_ext.hpp>

class Player;

class Quester : public IObserver
{

public:
	static ResourceManager* resourceman;
	static Player* player;
	static std::vector<Quest> quests;
	static std::map<int, std::string> enemies;
	static std::map<int, std::string> npcs;
	static std::map<int, std::unique_ptr<Item>>* items;

public:
	Quester();
	~Quester();

public:
	void Process(const int message, int id, int amount) override;

public:
	/*MAKE SURE TO RUN THIS ONLY ONCE AT THE START OF THE PROGRAM*/
	static void LoadQuests(int save);

	static void LoadPlayer(Player* playerptr);

	static void LoadEnemy(int id, const std::string& enemyName);

	static void LoadNPC(int id, const std::string npcName);

	static void LoadItem(std::map<int, std::unique_ptr<Item>>* itemTemplates);

	static void ClearAll();

private:
	void AnswerKillEnemy(int id, int amount = 1);

	void AnswerGatherItem(int id, int amount = 1);

	void AnswerTalkToNPC(int id);

	void AnswerGiveQuest(int id);

	void AnswerCompleteQuest(int id);
};

#endif
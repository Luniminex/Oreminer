#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "Observer.hpp"
#include "managers/ResourceManager.hpp"
#include <rapidxml-1.13/rapidxml_ext.hpp>

struct StatisticsContainer
{

	//total NO enemies killed
	int enemiesKilled;
	//keeps track of how many specific enemies were killed - ID, number
	std::map<int, int> uniqueEnemiesKilled;
	// keeps track which npcs were seen and which not - ID, seen/ not seenn
	std::map<int, bool> npcsSeen;
	//keeps track which quest have been completed and which not - ID quest, false/true
	std::map<int, bool> questCompleted;
};

class Statistics : public IObserver
{

private:
	static ResourceManager* resourceman;

	static StatisticsContainer statCont;

public:
	Statistics();
	~Statistics();

	static StatisticsContainer& GetStatisticsContainer();

	void Process(const int message, int id, int amount = 1) override;

	static void LoadStatistics(int save);

	static void ClearAll();
};

#endif
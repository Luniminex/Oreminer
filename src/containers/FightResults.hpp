#ifndef FIGHTRESULTS_HPP
#define FIGHTRESULTS_HPP

#include "containers/EnemyData.hpp"

class FightResults
{

private:
	bool playerWon;
	bool fightEnded;

	//ID, Amount
	std::vector<std::pair<int, int>> itemsDropped;
	int expDropped;

public:
	FightResults();
	~FightResults();

public:
	bool PlayerWon();
	bool FightEnded();
	std::vector<std::pair<int, int>>& GetItemsDropped();
	int GetExperienceDropped();

public:
	void SetWhoWon(bool playerWon);
	void SetFightEnded(bool fightEnded);
	void AddItemDrop(ItemDrop& drop);
	void SetExpDrop(int experience);
};

#endif
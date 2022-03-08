#include "containers/FightResults.hpp"

FightResults::FightResults()
{
	playerWon = false;
	fightEnded = false;
}

FightResults::~FightResults()
{
}

bool FightResults::PlayerWon()
{
	return playerWon;
}

bool FightResults::FightEnded()
{
	return fightEnded;
}

std::vector<std::pair<int, int>>& FightResults::GetItemsDropped()
{
	return itemsDropped;
}

int FightResults::GetExperienceDropped()
{
	return expDropped;
}

// true for player, false for enemy
void FightResults::SetWhoWon(bool playerWon)
{
	this->playerWon = playerWon;
}

void FightResults::SetFightEnded(bool fightEnded)
{
	this->fightEnded = fightEnded;
}

void FightResults::AddItemDrop(ItemDrop& drop)
{

	if ((std::rand() % 100 + 1) < drop.dropChance)
	{ //item dropped
		int amountDropped = 0;
		amountDropped = drop.min + (std::rand() % drop.max);
		itemsDropped.push_back(std::pair(drop.id, amountDropped));
		std::cout << amountDropped << " Items were dropped by enemy with ID: " << drop.id << std::endl;
	}
	else // item did not drop
	{
		std::cout << "Item dropped by enemy with ID: " << drop.id << " didnt drop" << std::endl;
	}
}

void FightResults::SetExpDrop(int experience)
{
	this->expDropped = experience;
}

#include "Quest.hpp"

Quest::Quest(const int id, int state, const int unlockCondition, const int type, std::vector<int> goalID, std::vector<int> goalAmounts,
	std::vector<int> goalDone, std::vector<int> rewardID, std::vector<int> rewardAmount, std::string name, std::string context)
{
	this->id = id;
	this->state = state;
	this->unlockCondition = unlockCondition;
	this->type = type;
	this->GoalID = goalID;
	this->GoalAmounts = goalAmounts;
	this->GoalDone = goalDone;
	this->rewardID = rewardID;
	this->rewardAmount = rewardAmount;
	this->name = name;
	this->context = context;
}
#ifndef QUEST_HPP
#define QUEST_HPP

enum QuestStates
{
	pending = 0,
	unlocked = 1,
	inProgress = 2,
	done = 3
};

enum QuestUnlockCondition
{
	allBeforeCompleted = 0
};

enum QuestTypes
{
	KILL,
	TALK,
	GATHER
};

struct Quest
{

	Quest(const int id, int state, const int unlockCondition, const int type, std::vector<int> goalID, std::vector<int> goalAmounts,
		std::vector<int> goalDone, std::vector<int> rewardID, std::vector<int> rewardAmount, std::string name, std::string context);

	int id;
	int state;
	int unlockCondition;
	int type;

	std::vector<int> GoalID;
	std::vector<int> GoalAmounts;
	std::vector<int> GoalDone;

	std::vector<int> rewardID;
	std::vector<int> rewardAmount;

	std::string name;
	std::string context;
};

#endif

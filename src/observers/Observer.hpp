#ifndef OBSERVER_HPP
#define OBSERVER_HPP

enum SubjectActions
{
	KILL_ENEMY,
	GATHER_ITEM,
	GIVE_QUEST,
	COMPLETE_QUEST,
	UNLOCK_QUEST,
	TALK_TO_NPC,
	NEXT_PAGE,
	ORE_MINED,
	ENTER_MAINMENU,
	ENTER_GAMESTATE,
	ENTER_FIGHT,
	ENTER_SETTINGS,
	BTN_CLICKED,
	LEVEL_UP
};

class IObserver
{

public:
	virtual void Process(const int message, int id, int amount = 1) = 0;
};

class Subject
{
private:
	std::list<std::shared_ptr<IObserver>> observers;

public:
	void RegisterObserver(std::shared_ptr<IObserver> observer)
	{
		observers.push_back(observer);
	}

	void UnregisterObserver(std::shared_ptr<IObserver> observer)
	{
		observers.remove(observer);
	}

	void NotifyObservers(const int message, int id = -1)
	{
		for (auto& a : observers)
		{
			a->Process(message, id);
		}
	}
};

#endif
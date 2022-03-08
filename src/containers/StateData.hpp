#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include "containers/GraphicsContainer.hpp"
#include "states/State.hpp"
#include <map>
#include <stack>

class State;

class StateData
{

public:
	sf::RenderWindow* window;				   //ukazatel na okno aplikace
	GraphicsContainer graphicsContainer;	   //struktura pro uchovávání informací o okně aplikace
	std::map<std::string, int>* supportedKeys; //ukazatel na podporované klávesy
	std::stack<State*>* states;				   // ukazatel na stack stavů
	int gridSize;							   //velkikost jednoho pole na mřízce
	int save = 1;							   //číslo slotu aktivního uloženého profilu

public:
	StateData();
};

#endif
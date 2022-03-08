#include "State.hpp"

//constructors and destructors
State::State(StateData& stateData) :
	stateData(stateData)
{

	this->paused = false;
	this->wantsToEnd = false;
	this->keyTime = 0.f;
	this->keyTimeMax = 15.f;

	this->mouseWheelDelta = 0;
	std::cout << "Started state" << std::endl;
}

State::~State()
{
}

const bool& State::WantsQuit()
{
	return wantsToEnd;
}

int State::GetMouseWheelDelta()
{
	int temp = mouseWheelDelta;
	mouseWheelDelta = 0;
	return temp;
}

void State::UpdateKeyTime(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

void State::SetMouseWheelDelta(int msDelta)
{
	this->mouseWheelDelta = msDelta;
}

void State::UpdateMosPos(sf::View* view)
{
	mousePosScreen = sf::Mouse::getPosition();														//gets desktop screen pos
	mousePosWindow = sf::Mouse::getPosition(*stateData.window);										//gets window screen pos
	mousePosWindow = static_cast<sf::Vector2i>(stateData.window->mapPixelToCoords(mousePosWindow)); /* had probles when resizing windows - wasn't able to click buttons properly - worked after did this, dunno why*/

	if (view) //if ther is a view, sets view for calculating mouse position in view
		stateData.window->setView(*view);

	mousePosView = stateData.window->mapPixelToCoords(mousePosWindow); //gets current view mouse pos
	mousePosGrid = sf::Vector2i(
		static_cast<int>(mousePosView.x) / stateData.gridSize,
		static_cast<int>(mousePosView.y) / stateData.gridSize);

	stateData.window->setView(stateData.window->getDefaultView()); //sets back view to default
}
//protected functions
void State::Pause()
{
	this->paused = !paused;
}

bool State::IsPaused()
{
	return paused;
}

bool State::GetKeyTime()
{
	if (keyTime >= keyTimeMax)
	{ //pokud je aktuální keyTime větší než hranice k překročení, keyTimeMax
		keyTime = 0.f;
		return true; //tak se vrátí pravda
	}
	return false;
}

//public functions

//private functions

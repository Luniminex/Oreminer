#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "GUI/GUI.hpp"
#include "states/EditorState.hpp"
#include "states/GameState.hpp"

class MainMenuState : public State, Subject
{
	//resources
private:
	ResourceManager* resourceman = ResourceManager::GetInstance();

	sf::Font font;
	sf::Text credits;
	std::map<std::string, std::unique_ptr<GUI::Button>> btns;
	//constructors
public:
	//constructor for gamestate
	MainMenuState(StateData& stateData);

	~MainMenuState();

	//initializers
private:
	//initializes keybinds from supported keys
	void InitKeybinds();

	//loads all fonts
	void InitFonts();

	//initializes buttons
	void InitButtons();

	//public functions
public:
	//initializes gamestate
	void InitState();

	//reinits some stuff if needed
	void AfterInit();

	//checks if should quit
	void CheckQuit();

	//gets input
	void Input();

	//updates buttons
	void UpdateButtons();

	//updates gamestate every frame if active
	void Update(float dt);

	//renders to render window
	void Display(sf::RenderTarget* target = nullptr);

	//invoked everytime gamestate ends
	void EndState();
};

#endif
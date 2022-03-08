#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include <map>

#include "GUI/GUI.hpp"
#include "states/State.hpp"
#include "managers/ResourceManager.hpp"

class SettingsState : public State, Subject
{
	//resources
public:
	ResourceManager* resourcman = ResourceManager::GetInstance();

	sf::Font font;

	std::map<std::string, std::unique_ptr<GUI::Button>> btns;			 //holds all buttons
	std::map<std::string, std::unique_ptr<GUI::DropdownList>> drpdLists; //holds all dropdownLists

	//!!!!!!! později předělat až se ti bude chtít, bo to je na nic - nesupportuje všechny obrazovky, jen ty které tam dáš
	std::map<std::string, sf::Vector2u> resolutions; //holds all resolutions

	std::map<std::string, std::unique_ptr<GUI::Checkbox>> checkboxes;
	//constructors
public:
	SettingsState(StateData& stateData);
	~SettingsState();
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

	//updates gamestate every frame if active
	void Update(float dt);

	//renders to render window
	void Display(sf::RenderTarget* target = nullptr);

	//invoked everytime gamestate ends
	void EndState();
	//private functions
private:
	//inits all fonts
	void InitFont();

	//inits keybinds
	void InitKeybinds();

	//inits gui elements
	void InitButtons();

	//updates all gui elements
	void UpdateButtons(float dt);

	//applies all settings selected in settings menu
	void ApplySettings();
};

#endif
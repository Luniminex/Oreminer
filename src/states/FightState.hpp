#ifndef FIGHTSTATE_HPP
#define FIGHTSTATE_HPP

#include <iostream>

#include "GUI/PauseMenu.hpp"
#include "containers/FightResults.hpp"
#include "managers/CombatManager.hpp"
#include "map/Tilemap.hpp"
#include "states/SettingsState.hpp"
#include "states/State.hpp"

class State;
class CombatManager;
class FightResults;

class FightState : public State, Subject
{
	//resources
private:
	ResourceManager* resourceman = ResourceManager::GetInstance();
	CombatManager* combatMan; //later remove pointer

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Font font;
	sf::View view;
	sf::Text debugText;

	PauseMenu pausemenu;

	sf::RectangleShape bgr;

	sf::Texture playerTexture; //temporal solution for combar man enemy fighter anim component
	sf::Texture enemyTexture;  //temporal solution for combar man enemy fighter anim component

	std::map<std::string, sf::Texture> textures; //textures for this game state

	int mapType;

	//vars
public:
	//constructors
public:
	FightState(StateData& stateData, Player& player, EnemyData enemy, std::map<int, std::unique_ptr<Item>>& items, sf::Texture& itemSheet, FightResults& results, int mapType);
	~FightState();

	//accessors
public:
	//modifiers
public:
	//public functions
public:
	//contains initializing functions
	void InitState();

	//re-inits stuff, lie GUI
	void AfterInit();

	//Check for quit
	void CheckQuit();

	//gets input
	void Input();

	//updates state
	void Update(float dt);

	//displays everything from state
	void Display(sf::RenderTarget* target = nullptr);

	//does something when state ends
	void EndState();

	//private functions
private:
	//loads keybinds for this state
	void InitKeybinds(const std::string& path);

	//loads fonts
	void InitFont(const std::string& path);

	//inits GUI
	void InitGUI();

	//loads textures
	void LoadTextures();

	//initilaizes deferred rendering
	void InitDeferredRendering();

	//updates GUI
	void UpdateGUI(float dt);
};

#endif
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <cstring>
#include <list>
#include <rapidxml-1.13/rapidxml_ext.hpp>

#include "States/state.hpp"

#include "GUI/PauseMenu.hpp"
#include "States/MainMenuState.hpp"
#include "containers/Recipe.hpp"
#include "items/Item.hpp"
#include "managers/DialogueManager.hpp"
#include "map/Tilemap.hpp"
#include "states/FightState.hpp"
#include "states/SettingsState.hpp"

class Player;
class PauseMenu;
class Tilemap;
class Item;
class ResourceManager;
class DialogueManager;
class Subject;

class GameState : public State, public Subject
{

	//resources
private:
	ResourceManager* resourceman = ResourceManager::GetInstance();
	DialogueManager* DialogueManager = DialogueManager::GetInstance();

	FightResults results;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Font font;
	sf::View view;


	Player* player;
	PauseMenu pausemenu;

	Tilemap tileMap;

	std::map<std::string, sf::Texture> textures;		//textures for this game state
	std::map<int, std::unique_ptr<Item>> itemTemplates; //holds all pre loaded items in game
	std::map<int, std::vector<Recipe>> recipes;			//holds all crafting interfaces recipes

	//constructors
public:
	//constructor for gamestate
	GameState(StateData& stateData);

	~GameState();
	//initializers
private:
	//loads all fonts
	void InitFonts();

	//loads all necessary textures
	void InitTextures();

	//initalizes keybinds from supported keys
	void InitKeybinds();

	//inits view
	void InitView();

	//inits deferred render
	void InitDeferredRendering();

	//inits GUI
	void InitGUI();

	//inits items
	void InitItems();

	//inits recipes for crafting interfaces
	void InitRecipes();

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
	void UpdateButtons(float dt);

	//updates gamestate every frame if active
	void Update(float dt);

	//renders to render window
	void Display(sf::RenderTarget* target = nullptr);

	//invoked everytime gamestate ends
	void EndState();

private:
	void CheckFightEnded();

	void UpdateMap(float dt);

	void LoadSave(int save);

	void RegisterObservers();
};

#endif
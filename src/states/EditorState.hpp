#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include <memory>
#include <string>

#include "GUI/GUI.hpp"
#include "GUI/PauseMenu.hpp"
#include "map/Tilemap.hpp"
#include "states/SettingsState.hpp"
#include "states/State.hpp"

class EditorState : public State
{
	//resources
public:
private:
	ResourceManager* resourceman = ResourceManager::GetInstance(); //ukazatel na instanci manažera zdrojů

	sf::Font font; // aktuální font, který se používá

	sf::RenderTexture renderTexture; // textura pro opožděné renderování
	sf::Sprite renderSprite;		 //sprite pro opožděné renderování
	sf::View view;					 //2D kamera, která říká, co vidíme na obrazovce, možnost nastavení přiblížení

	sf::RectangleShape selector;   //ukazatel vybraného pole na mřížce
	sf::IntRect selectedTile;	   // pozice na textuře vybrané dlaždice
	sf::RectangleShape mapBorders; //indikuje hranice mapy

	int tileTypeSelected;  //typ vybrané dlaždice
	unsigned int selLayer; //číslo vrstvy

	float zoomLevel;		// úroveň přibližení
	float cameraSpeed;		//rychlost pohybu kamery
	sf::Vector2f cameraPos; //pozice kamery

	bool singleLayerMode; //mód pro zobrazování jednotné vrstvy
	bool enableCollision; //mód pokládání boxů kolize
	bool tileReplace;	  // mód překládání dlažeb

	sf::RectangleShape sideBar;												   //hraniční box pro GUI elementy
	sf::Text info;															   //text, obsahující informace o pozici, vrstvě atd..
	std::map<std::string, std::unique_ptr<GUI::TextureSelector>> texSelectors; //mapa dostupných tlačítek pro vybárání dlaždic
	std::map<std::string, std::unique_ptr<GUI::Button>> btns;				   //mapa všech tlačítek

	Tilemap map; //objekt mapy

	PauseMenu pauseMenu; //objekt menu pro pozastavení hry
public:
	EditorState(StateData& stateData);
	~EditorState();

	//functions
public:
	//inits keybinds
	void InitKeybinds();

	//inits font
	void InitFont();

	//inits view
	void InitView();

	//inits deferred render
	void InitDeferredRender();

	//inits gui elements
	void InitGUI();

	//initializes gamestate
	void InitState();

	//reinits some stuff if needed
	void AfterInit();

	//checks if should quit
	void CheckQuit();

	//checks input of buttons
	void InputGUI();

	//checks for input for editor modes and disables/enables em
	void InputModes();

	//checks users input
	void InputPlayer(float dt);

	//input
	void Input();

	//updates text which contains info about certain things
	void UpdateInfoText();

	//updates pause menu buttons
	void UpdatePauseMenu(float dt);

	//updates gui
	void UpdateGUI(float dt);

	//updates selector
	void UpdateSelector();

	//updates gamestate every frame if active
	void Update(float dt);

	//renders all gui elements to window
	void DisplayGUI();

	//renders to render window
	void Display(sf::RenderTarget* target = nullptr);

	//invoked everytime gamestate ends
	void EndState();

private:
	void CheckInputSelectors();
};

#endif
#ifndef STATE_HPP
#define STATE_HPP

#include "ESC/entities/Player.hpp"
#include "containers/StateData.hpp"
#include "observers/AudioPlayer.hpp"
#include "observers/Quester.hpp"
#include "observers/Statistics.hpp"

class StateData;
class GraphicsContainer;

class State
{
protected:
	StateData& stateData; //data potřebné k přecházenní ze stavů

	std::map<std::string, int> keybinds; //mapa kláves k určitým akcím v daném stavu

	sf::Vector2i mousePosScreen; //globální pozice myši
	sf::Vector2i mousePosWindow; //pozice myši v okně
	sf::Vector2f mousePosView;	 //pozice myši v zobrazení
	sf::Vector2i mousePosGrid;	 //pozice myši na mřížce

	int mouseWheelDelta;

	bool paused;	 //říká, jestli je hra pozastavena
	bool wantsToEnd; // říká, jestli stac chce ukončit

	float keyTime;	  //aktuální prodlení
	float keyTimeMax; //maximální prodlení mezi stisky kláves

public:
	State(StateData& stateData);

	virtual ~State();

	//vrátí pravdu, pokud se stala akce, kvůli které se stav chce ukončit
	const bool& WantsQuit();

	//vrátí deltu kolečka myši
	int GetMouseWheelDelta();

	//nastaví deltu kolečka myši
	virtual void SetMouseWheelDelta(int msDelta);

	//obsahuje všechny důležité funkce pro inicializaci stavu
	virtual void InitState() = 0;

	//funkce pro reinitializaci určitých prvků
	virtual void AfterInit() = 0;

	//aktualizuje časovač kláves
	virtual void UpdateKeyTime(float dt);

	//aktualizuje pozice myši
	virtual void UpdateMosPos(sf::View* view = nullptr);

	//zjistí, jestli se stav chce ukončit
	virtual void CheckQuit() = 0;

	//aktualizuje vstup
	virtual void Input() = 0;

	//aktualizuje stav každý snímek obrazovky
	virtual void Update(float dt) = 0;

	//vykresluje ze stavu do okna aplikace každý snímek obrazovky
	virtual void Display(sf::RenderTarget* target = nullptr) = 0;

	//ukončí stav
	virtual void EndState() = 0;

protected:
	//sets pause to true
	void Pause();

	//returns paused
	bool IsPaused();

	//returns true if you are able to press a key again
	bool GetKeyTime();

private:
};

#endif
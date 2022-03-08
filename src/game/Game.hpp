#include "ctime"

#include "containers/StateData.hpp"
#include "observers/Statistics.hpp"
#include "states/MainMenuState.hpp"

//třída, která se stará o chod aplikace
class Game
{
private:
	ResourceManager* resourceman = ResourceManager::GetInstance();//získaní instance manažere zdrojů

	sf::RenderWindow* window; //hlavní okno aplikace
	sf::Event evnt;			  //proměnná pro získaní SFML eventu

	StateData stateData; // kontejner, který drží data důležité pro přecházení ze stavů

	sf::Clock clockDT; //hodiny delta time
	float dt;		   //delta time - uběhnutý čas za jeden snímek

	int gridSize; //určuje velikost jednoho pole ve hře

	std::stack<State*> states;				  //drží v sobě všechny stavy
	std::map<std::string, int> supportedKeys; //mapa pro načtení všech podporovaých kláves

public:
	Game();

	void Run();

	~Game();

private:
	void InitWindow();

	void InitKeys();

	void InitStates();

	void InitStateData();

	void InitGame();

	void UpdateDT();

	void UpdateEvents();

	void Update();

	void Display();

	void EndApplication();
};
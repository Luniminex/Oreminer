#ifndef GAME_HPP
#define GAME_HPP

#include "Game.hpp"

//Konstruktor
Game::Game()
{
	//inicializace náhodného seedu podle času, pro generaci náhodných čísel
	std::srand(std::time(nullptr));
}

//destruktor
Game::~Game()
{
	delete window; //smazání okna

	while (!states.empty()) //smyčka smaže každý stav ve stacku
	{
		delete states.top();
		states.pop();
	}
}

void Game::InitWindow()
{
	//nastaví výchozí nastavení
	stateData.graphicsContainer.SetWindowTitle("Title");
	stateData.graphicsContainer.width = 1920;
	stateData.graphicsContainer.height = 1080;
	stateData.graphicsContainer.bitsPerPixel = 32;
	stateData.graphicsContainer.SetFrameLimit(60);
	stateData.graphicsContainer.SetVSync(true);

	//otevře soubor, kde se nachází soubor s daty nastavení
	std::ifstream ifstr(resourceman->ConfigAt("WINDOWSETTINGS"));

	if (ifstr.is_open())
	{ //načte data ze souboru do grafického kontejneru
		std::getline(ifstr, stateData.graphicsContainer.windowTitle);
		ifstr >> stateData.graphicsContainer.width
			>> stateData.graphicsContainer.height;

		ifstr >> stateData.graphicsContainer.frameLimit;
		ifstr >> stateData.graphicsContainer.vsync;
		ifstr >> stateData.graphicsContainer.fullscreen;
	}

	ifstr.close();

	//dočasná proměnná, do které se načtou informace, jak má být okno vytvořeno
	sf::VideoMode vidmod;

	vidmod.width = stateData.graphicsContainer.width;
	vidmod.height = stateData.graphicsContainer.height;
	vidmod.bitsPerPixel = stateData.graphicsContainer.bitsPerPixel;
	if (stateData.graphicsContainer.fullscreen == true)
	{
		window = new sf::RenderWindow(vidmod, stateData.graphicsContainer.windowTitle, sf::Style::Fullscreen);
	}
	else
	{
		window = new sf::RenderWindow(vidmod, stateData.graphicsContainer.windowTitle);
	}
	window->setFramerateLimit(stateData.graphicsContainer.frameLimit);
	window->setVerticalSyncEnabled(stateData.graphicsContainer.vsync);
}

void Game::InitKeys()
{
	//otevře soubor, kde je seznam všechn podporavaných kláves
	std::fstream ifstr(resourceman->ConfigAt("SUPPORTEDKEYS"));
	std::string key = "";
	int code = 0;

	while (ifstr >> key >> code)
	{ //vloží do mapy jméno a ke jménu kód klávesy
		supportedKeys[key] = code;
	}

	ifstr.close();
}

void Game::InitStates()
{
	states.push(new MainMenuState(stateData)); //temporary, testing states
}

void Game::InitStateData()
{
	stateData.window = window;
	stateData.states = &states;
	stateData.supportedKeys = &supportedKeys;
	stateData.gridSize = gridSize;
}

void Game::InitGame()
{
	gridSize = 32;

	InitWindow();
	InitKeys();
	InitStateData();
	InitStates();
}

void Game::Run()
{
	InitGame(); //všechny potřebné inicializace

	//herní smyčka
	while (window->isOpen()) //dokud je okno otevřené
	{
		Update();  //aktualizace stavů
		Display(); //vykreslování ze stavů
	}
}

void Game::UpdateDT()
{
	dt = clockDT.restart().asSeconds(); // restartuje hodiny a vrátí čas delta
}

void Game::UpdateEvents()
{
	while (window->pollEvent(evnt))
	{ //chytne událost
		if (evnt.type == sf::Event::Closed)
		{ //zavře okno, pokud zavře okno
			window->close();
		}
		if (evnt.type == sf::Event::MouseWheelScrolled)
		{ //zachytí vstup z kolečka myši
			states.top()->SetMouseWheelDelta(static_cast<int>(evnt.mouseWheelScroll.delta));
		}
	}
}

void Game::Update()
{
	UpdateDT();
	UpdateEvents();
	if (!states.empty())
	{ //dokud není stack prázdný, aktulizuje vrchní stav
		states.top()->Update(dt);
	}
	else
	{ // pokud je stack prázdný, ukončí se aplikace
		EndApplication();
	}
}

void Game::Display()
{
	window->clear(); //prvně se musí vyčistit okno

	if (!states.empty()) // pokud není stackc prázdný, tak může vykreslovat
	{
		states.top()->Display();

		if (states.top()->WantsQuit())
		{ //pokud nějaký stav žádá o ukonečení, tak se stav ukončí
			//a automaticky se aktualizuje stav podním
			states.top()->EndState();
			states.pop();
			if (!states.empty())
			{
				states.top()->AfterInit(); //funkce stavů, pro reinitilizaci grafického rozhraní
			}
		}
	}

	window->display();
}

void Game::EndApplication()
{
	std::cout << "Exiting application";
	window->close();
}

#endif

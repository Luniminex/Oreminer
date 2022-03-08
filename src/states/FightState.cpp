#include "states/FightState.hpp"

/****************constructors / destructors****************/

FightState::FightState(StateData& stateData, Player& player, EnemyData enemy, std::map<int, std::unique_ptr<Item>>& items, sf::Texture& itemSheet, FightResults& results, int mapType) :
	State(stateData),
	pausemenu(stateData.graphicsContainer.GetSize())
{
	this->mapType = mapType;
	InitState();

	std::cout << enemy.fightsceneAnimPath << std::endl;
	if (!enemyTexture.loadFromFile(enemy.fightsceneAnimPath))
	{
		std::cout << "Failed to load enemy texture in combat manager" << std::endl;
	}

	std::cout << player.fightsceneTexPath << std::endl;
	if (!playerTexture.loadFromFile(player.fightsceneTexPath))
	{
		std::cout << "Failed to load enemy texture in combat manager" << std::endl;
	}

	combatMan = new CombatManager(player, enemy, playerTexture, enemyTexture, items, itemSheet, results, font, stateData.graphicsContainer.GetSize());
	std::cout << "Started Fight state" << std::endl;
}

FightState::~FightState()
{
	std::cout << "Deleted FightState" << std::endl;
}

void FightState::InitState()
{
	InitKeybinds(resourceman->SaveDataAt(stateData.save, "FIGHTKEYBINDS"));
	InitFont(resourceman->ConfigAt("ROBOTOFONT"));
	LoadTextures();
	InitGUI();
	InitDeferredRendering();

	RegisterObserver(std::make_shared<AudioPlayer>());
	NotifyObservers(SubjectActions::ENTER_FIGHT, mapType);

	bgr.setTexture(&textures.at("BACKGROUND_TEXTURE"));
	bgr.setSize(stateData.graphicsContainer.GetSize());
}

void FightState::AfterInit()
{
	NotifyObservers(SubjectActions::ENTER_FIGHT);
	pausemenu.AfterInit(stateData.graphicsContainer.GetSize());
	bgr.setSize(stateData.graphicsContainer.GetSize());
	combatMan->AfterInit(stateData.graphicsContainer.GetSize());
}

void FightState::CheckQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("PAUSE"))) && GetKeyTime() == true)
	{
		std::cout << "Paused" << std::endl;
		Pause();
	}
}

void FightState::Input()
{
	CheckQuit();
}

void FightState::Update(float dt)
{
	UpdateKeyTime(dt);
	UpdateMosPos(&view);
	Input();

	if (!IsPaused())
	{
		combatMan->Update(dt, mousePosWindow);
		if (combatMan->GetFightResults() == true)
		{
			wantsToEnd = true;
		}
	}
	else
	{
		UpdateGUI(dt);
	}
}

void FightState::Display(sf::RenderTarget* target)
{
	if (!target)
	{ // if no target specified use window from constructor
		target = stateData.window;
	}
	renderTexture.clear();

	renderTexture.draw(bgr);

	combatMan->Display(renderTexture);
	combatMan->DisplayGUI(renderTexture);

	if (IsPaused())
	{ //if paused, draw pause menu
		pausemenu.Display(renderTexture);
	}

	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	target->draw(renderSprite);
}

void FightState::EndState()
{
	std::cout << "Ending FightState" << std::endl;
	delete combatMan;
	std::cout << "Fighter state ended" << std::endl;
}

/**************private functions************/
void FightState::InitKeybinds(const std::string& path)
{
	std::ifstream ifstr(path);
	std::string action;
	std::string key;

	while (ifstr >> action >> key)
	{
		keybinds[action] = stateData.supportedKeys->at(key); //sets action to specific key
	}
	ifstr.close();
}

void FightState::InitFont(const std::string& path)
{
	if (!font.loadFromFile(path))
	{
		std::cout << "Could not load font in fightstate/initfonts" << std::endl;
	}
}

void FightState::InitGUI()
{
	pausemenu.AddButton("RESUME_BTN", "Resume", 13.8f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("SETTINGS_BTN", "Settings", 27.7f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("SAVE_BTN", "Save and Quit", 41.6f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("QUIT_BTN", "Quit", 55.2f, sf::Vector2f(10.4f, 7.5f), font);
}

void FightState::LoadTextures()
{
	sf::Texture temp;
	switch (mapType)
	{
		case mapTypes::forest:
			if (!temp.loadFromFile(resourceman->TextureAt("FORESTBGR")))
			{
				std::cout << "FIGHTSTATE:: FAILED TO LOAD BACKGROUND TEXTURE" << std::endl;
			}
			break;
		case mapTypes::cave:
			if (!temp.loadFromFile(resourceman->TextureAt("CAVEBGR")))
			{
				std::cout << "FIGHTSTATE:: FAILED TO LOAD BACKGROUND TEXTURE" << std::endl;
			}
			break;
		default:
			if (!temp.loadFromFile(resourceman->TextureAt("FORESTBGR")))
			{
				std::cout << "FIGHTSTATE:: FAILED TO LOAD BACKGROUND TEXTURE" << std::endl;
			}
			break;
	};

	textures["BACKGROUND_TEXTURE"] = temp;
}

void FightState::InitDeferredRendering()
{
	renderTexture.create(stateData.graphicsContainer.width, stateData.graphicsContainer.height);

	renderSprite.setTexture(renderTexture.getTexture());
	renderSprite.setTextureRect(sf::IntRect(0, 0, stateData.graphicsContainer.width, stateData.graphicsContainer.height));
}

void FightState::UpdateGUI(float dt)
{
	pausemenu.Update(mousePosWindow, dt);

	if (pausemenu.GetButton("RESUME_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		Pause();
	}

	if (pausemenu.GetButton("SETTINGS_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		stateData.states->push(new SettingsState(stateData));
	}

	if (pausemenu.GetButton("SAVE_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		wantsToEnd = true;
	}

	if (pausemenu.GetButton("QUIT_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		wantsToEnd = true;
	}
}

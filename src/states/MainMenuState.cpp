#include <states/MainMenuState.hpp>

//constructors and destructors

MainMenuState::MainMenuState(StateData& stateData) :
	State(stateData)
{
	InitState();
}

MainMenuState::~MainMenuState()
{
	std::cout << "Deleted Main menu state" << std::endl;
}

//public functions

void MainMenuState::InitKeybinds()
{
	std::ifstream ifstr(resourceman->ConfigAt("MAINMENUKEYBINDS"));
	std::string action;
	std::string key;

	while (ifstr >> action >> key) // loads all keys
	{
		keybinds[action] = stateData.supportedKeys->at(key);
	}
	ifstr.close();
	for (auto i : keybinds)
	{ //DEBUG
		std::cout << i.first << "-" << i.second << std::endl;
	}
}

void MainMenuState::InitFonts()
{
	font.loadFromFile(resourceman->ConfigAt("ROBOTOFONT"));
}

void MainMenuState::InitButtons()
{

	std::cout << "Resolution : " << stateData.graphicsContainer.width << ", " << stateData.graphicsContainer.height << std::endl;
	btns["NEW_GAME_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(5.2f, 9.2f, sf::Vector2f(13.f, 9.2f), font, "Play Game", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["EDITOR_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(5.2f, 27.7f, sf::Vector2f(13.f, 9.2f), font, "Editor", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["SETTINGS_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(5.2f, 46.2f, sf::Vector2f(13.f, 9.2f), font, "Settings", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["EXIT_GAME_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(5.2f, 64.4, sf::Vector2f(13.f, 9.2f), font, "Exit", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["SAVE_FILE_1"] = std::unique_ptr<GUI::Button>(new GUI::Button(25.2f, 15.f, sf::Vector2f(10.f, 15.f), font, "Save 1", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["SAVE_FILE_2"] = std::unique_ptr<GUI::Button>(new GUI::Button(45.2f, 15.f, sf::Vector2f(10.f, 15.f), font, "Save 2", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["SAVE_FILE_3"] = std::unique_ptr<GUI::Button>(new GUI::Button(65.2f, 15.f, sf::Vector2f(10.f, 15.f), font, "Save 3", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));

	btns.at("SAVE_FILE_1")->SetShown(false);
	btns.at("SAVE_FILE_2")->SetShown(false);
	btns.at("SAVE_FILE_3")->SetShown(false);
}

void MainMenuState::InitState()
{
	InitKeybinds();
	InitFonts();

	credits.setFont(font);
	credits.setCharacterSize(GUI::CaclCharSizeSmall(stateData.graphicsContainer.GetSize()));
	credits.setString("Music created by Boris Patloch\n and Andrej Varga.\n Game made by Jakub Smieja");
	credits.setPosition(GUI::PeToPiX(85.f, stateData.graphicsContainer.GetSize().x), GUI::PeToPiY(92.f, stateData.graphicsContainer.GetSize().y));

	RegisterObserver(std::make_shared<AudioPlayer>());
	AudioPlayer::LoadSounds();
	GUI::TexturedGUI::LoadTextures();

	InitButtons();
	NotifyObservers(SubjectActions::ENTER_MAINMENU);
	std::cout << "Started MainMenuState" << std::endl;
}

void MainMenuState::AfterInit()
{
	NotifyObservers(SubjectActions::ENTER_MAINMENU);
	for (auto& a : btns)
	{
		a.second->AferInit(stateData.graphicsContainer.GetSize());
	}
	credits.setCharacterSize(GUI::CaclCharSizeSmall(stateData.graphicsContainer.GetSize()));
	credits.setPosition(GUI::PeToPiX(85.f, stateData.graphicsContainer.GetSize().x), GUI::PeToPiY(92.f, stateData.graphicsContainer.GetSize().y));
}

void MainMenuState::CheckQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("EXIT"))))
	{
		wantsToEnd = true; //exit when EXIT command is pressed on keyboard - make sure to bind in keybind file
	}
}

void MainMenuState::Input()
{
	CheckQuit();
}

void MainMenuState::UpdateButtons()
{
	for (auto& btn : btns)
	{ //iterates through buttons inside map and updates them
		btn.second->Update(mousePosWindow);
	}

	if (btns["NEW_GAME_BTN"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		if (btns.at("SAVE_FILE_1")->IsShown() == false)
		{
			btns.at("SAVE_FILE_1")->SetShown(true);
			btns.at("SAVE_FILE_2")->SetShown(true);
			btns.at("SAVE_FILE_3")->SetShown(true);
		}
		else
		{
			btns.at("SAVE_FILE_1")->SetShown(false);
			btns.at("SAVE_FILE_2")->SetShown(false);
			btns.at("SAVE_FILE_3")->SetShown(false);
		}
	}

	if (btns["SAVE_FILE_1"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		stateData.save = 1;
		stateData.states->push(new GameState(stateData));
	}
	if (btns["SAVE_FILE_2"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		stateData.save = 2;
		stateData.states->push(new GameState(stateData));
	}
	if (btns["SAVE_FILE_3"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		stateData.save = 3;
		stateData.states->push(new GameState(stateData));
	}
	if (btns["EDITOR_BTN"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		stateData.states->push(new EditorState(stateData));
	}

	if (btns["SETTINGS_BTN"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		stateData.states->push(new SettingsState(stateData));
	}

	if (btns["EXIT_GAME_BTN"]->GetState() == btnStates::BTN_PRESSED && GetKeyTime() == true)
	{
		wantsToEnd = true; //exit when exit btn is pressed
	}
}

void MainMenuState::Update(float dt)
{
	Input();
	UpdateMosPos();
	UpdateButtons();
	UpdateKeyTime(dt);

	(void)dt; //temp
}

void MainMenuState::Display(sf::RenderTarget* target)
{
	if (!target) // if target isn't specified, defaults to the window
		target = stateData.window;

	for (auto& btn : btns)
	{ //iterates through buttons inside map and renders them
		btn.second->Display(*target);
	}
	target->draw(credits);
}

void MainMenuState::EndState()
{
}

//protected functions

//private functions

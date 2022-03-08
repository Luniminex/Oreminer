#include "states/SettingsState.hpp"

//constructors / destructors
SettingsState::SettingsState(StateData& stateData) :
	State(stateData)
{
	InitState();
}
SettingsState::~SettingsState()
{
	std::cout << "Deleted Settings state" << std::endl;
}

//public functions

void SettingsState::InitState()
{
	InitFont();
	InitKeybinds();
	InitButtons();

	RegisterObserver(std::make_shared<AudioPlayer>());

	NotifyObservers(SubjectActions::ENTER_SETTINGS);
}

void SettingsState::AfterInit()
{
}

void SettingsState::CheckQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("EXIT"))))
	{
		wantsToEnd = true; //exit when EXIT command is pressed on keyboard - make sure to bind in keybind file
	}
}

void SettingsState::Input()
{
	CheckQuit();
}

void SettingsState::Update(float dt)
{
	UpdateKeyTime(dt);
	UpdateMosPos();
	Input();
	UpdateButtons(dt);
}

void SettingsState::Display(sf::RenderTarget* target)
{
	if (!target) // if target isn't specified, defaults to the window
		target = stateData.window;

	for (auto& a : btns)
	{
		a.second->Display(*target);
	}

	for (auto& a : drpdLists)
	{
		a.second->Display(*target);
	}

	for (auto& a : checkboxes)
	{
		a.second->Display(*target);
	}
}

void SettingsState::EndState()
{
	std::cout << "Ended Settings state" << std::endl;
}

//private functions

void SettingsState::InitFont()
{
	if (!font.loadFromFile(resourcman->ConfigAt("ROBOTOFONT")))
	{
		std::cout << "SETTINGS_STATE_ERROR:FAILED TO LOAD FONT" << std::endl;
	}
}

void SettingsState::InitKeybinds()
{
	std::ifstream ifstr(resourcman->ConfigAt("SETTINGSSTATEKEYBINDS"));
	std::string action;
	std::string key;

	while (ifstr >> action >> key)
	{
		keybinds[action] = stateData.supportedKeys->at(key); //sets action to specific key
	}
	ifstr.close();
}

void SettingsState::InitButtons()
{
	btns["BACK_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(2.6f, 83.3f, sf::Vector2f(13.f, 9.2f), font, "Back", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));
	btns["APPLY_BTN"] = std::unique_ptr<GUI::Button>(new GUI::Button(2.6f, 69.4f, sf::Vector2f(13.f, 9.2f), font, "Apply", GUI::btnTypes::basic, stateData.graphicsContainer.GetSize()));

	std::vector<std::string> temp;

	temp.push_back("2560x1440"); //custom resolutions
	temp.push_back("1920x1080");
	temp.push_back("1366x768");
	temp.push_back("1280x720");

	resolutions["2560x1440"] = (sf::Vector2u(2560, 1440)); // so it can be used later in apply
	resolutions["1920x1080"] = (sf::Vector2u(1920, 1080));
	resolutions["1366x768"] = (sf::Vector2u(1366, 768));
	resolutions["1280x720"] = (sf::Vector2u(1280, 720));

	/* //Lists all avaialbe video modes
	std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();

	for(unsigned i = 0; i < videoModes.size();i++){
		temp.push_back(std::to_string(videoModes.at(i).width)+ "x" + std::to_string(videoModes.at(i).height));
	}
	*/

	drpdLists["RESOLUTIONS_DRPL"] = std::unique_ptr<GUI::DropdownList>(new GUI::DropdownList(18.2f, 9.2f, sf::Vector2f(7.8f, 4.6f), font, temp, stateData.graphicsContainer.GetSize()));

	checkboxes["FULLSCREEN_CHECKBOX"] = std::unique_ptr<GUI::Checkbox>(new GUI::Checkbox(1.f, 10.f, sf::Vector2f(2.08f, 3.7f), 1.f, stateData.graphicsContainer.fullscreen, font, "Fullscreen", 3.f, sf::Color(242, 240, 240), sf::Color(70, 73, 74, 100), sf::Color(242, 240, 240), sf::Color(70, 73, 74, 100), sf::Color(100, 100, 100), sf::Color(200, 200, 200), sf::Color(120, 120, 120), sf::Color(206, 212, 214), sf::Color(100, 100, 100), sf::Color(232, 232, 232), sf::Color::Transparent, sf::Color::Transparent, stateData.graphicsContainer.GetSize()));
	checkboxes["VSYNC_CHECKBOX"] = std::unique_ptr<GUI::Checkbox>(new GUI::Checkbox(1.f, 15.f, sf::Vector2f(2.08f, 3.7f), 1.f, stateData.graphicsContainer.vsync, font, "VSync", 3.f, sf::Color(242, 240, 240), sf::Color(70, 73, 74, 100), sf::Color(242, 240, 240), sf::Color(70, 73, 74, 100), sf::Color(100, 100, 100), sf::Color(200, 200, 200), sf::Color(120, 120, 120), sf::Color(206, 212, 214), sf::Color(100, 100, 100), sf::Color(232, 232, 232), sf::Color::Transparent, sf::Color::Transparent, stateData.graphicsContainer.GetSize()));
}

void SettingsState::UpdateButtons(float dt)
{

	for (auto& a : btns)
	{
		a.second->Update(mousePosWindow);
	}
	for (auto& a : drpdLists)
	{
		a.second->Update(dt, mousePosWindow);
	}

	for (auto& a : checkboxes)
	{
		a.second->Update(mousePosWindow, dt);
	}

	if (btns.at("BACK_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		wantsToEnd = true;
	}

	if (btns.at("APPLY_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		ApplySettings();
		btns.at("BACK_BTN")->AferInit(stateData.graphicsContainer.GetSize());

		btns.at("APPLY_BTN")->AferInit(stateData.graphicsContainer.GetSize());

		drpdLists.at("RESOLUTIONS_DRPL")->AferInit(stateData.graphicsContainer.GetSize());

		checkboxes.at("FULLSCREEN_CHECKBOX")->AferInit(stateData.graphicsContainer.GetSize());
		checkboxes.at("FULLSCREEN_CHECKBOX")->checked = stateData.graphicsContainer.fullscreen;

		checkboxes.at("VSYNC_CHECKBOX")->AferInit(stateData.graphicsContainer.GetSize());
		checkboxes.at("VSYNC_CHECKBOX")->checked = stateData.graphicsContainer.vsync;
	}
}

void SettingsState::ApplySettings()
{

	std::ifstream ifstr(resourcman->ConfigAt("WINDOWSETTINGS"));
	if (!ifstr)
	{
		std::cout << "SETTINGSSTATE/APPLYSETTINGS: FAILED TO OPEN WINDOW.INI" << std::endl;
	}

	if (ifstr.is_open()) // loads from file settings
	{
		std::getline(ifstr, stateData.graphicsContainer.windowTitle);
		ifstr >> stateData.graphicsContainer.width >> stateData.graphicsContainer.height;
		ifstr >> stateData.graphicsContainer.frameLimit;
		ifstr >> stateData.graphicsContainer.vsync;
		ifstr >> stateData.graphicsContainer.fullscreen;
	}

	std::cout << "Fullscreen: " << stateData.graphicsContainer.fullscreen;

	ifstr.close();

	//loads changed settings from settings state and then sets them

	stateData.graphicsContainer.width = resolutions.at(drpdLists.at("RESOLUTIONS_DRPL")->GetActiveText()).x;
	stateData.graphicsContainer.height = resolutions.at(drpdLists.at("RESOLUTIONS_DRPL")->GetActiveText()).y;
	stateData.graphicsContainer.fullscreen = checkboxes.at("FULLSCREEN_CHECKBOX")->checked;
	stateData.graphicsContainer.vsync = checkboxes.at("VSYNC_CHECKBOX")->checked;

	sf::VideoMode videomode;

	videomode.width = stateData.graphicsContainer.width;
	videomode.height = stateData.graphicsContainer.height;
	videomode.bitsPerPixel = stateData.graphicsContainer.bitsPerPixel;

	if (stateData.graphicsContainer.fullscreen == true)
	{
		stateData.window->create(videomode, stateData.graphicsContainer.windowTitle, sf::Style::Fullscreen);
	}
	else
	{
		stateData.window->create(videomode, stateData.graphicsContainer.windowTitle);
	}
	stateData.window->setFramerateLimit(stateData.graphicsContainer.frameLimit);
	stateData.window->setVerticalSyncEnabled(stateData.graphicsContainer.vsync);
}
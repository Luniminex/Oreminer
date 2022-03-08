#include "states/EditorState.hpp"

//constructors / destructors
EditorState::EditorState(StateData& stateData) :
	State(stateData),
	map(sf::Vector2u(120, 120), stateData.gridSize, 3),
	pauseMenu(stateData.graphicsContainer.GetSize())
{
	selLayer = 0;
	cameraSpeed = 500.f;
	cameraPos = { 0.f, 0.f };
	zoomLevel = 1.f;

	singleLayerMode = false;
	enableCollision = false;
	tileReplace = false;

	keyTimeMax = 10.f;

	InitState();
}
EditorState::~EditorState()
{
}

//public functions

void EditorState::InitKeybinds()
{
	std::ifstream ifstr(resourceman->SaveDataAt(stateData.save, "EDITORKEYBINDS")); //soubor, kde jsou definovány akce k jednotlivým klávesám
	std::string action;
	std::string key;

	while (ifstr >> action >> key) // načte všechny klávesy k dané akci
	{
		keybinds[action] = stateData.supportedKeys->at(key);
	}
	ifstr.close();
}

void EditorState::InitFont()
{
	if (!font.loadFromFile(resourceman->ConfigAt("ROBOTOFONT")))
	{
		std::cout << "EDITORSTATE::FAILED TO LOAD FONT" << std::endl;
	}
}

void EditorState::InitView()
{
	view.setSize(stateData.graphicsContainer.GetSize());									  //nastavení velikosti kamery podle velikosti okna
	view.setCenter(stateData.window->getSize().x / 2.f, stateData.window->getSize().y / 2.f); //nastavení centra kamery uprostřed okna
}

void EditorState::InitDeferredRender()
{
	renderTexture.create(stateData.graphicsContainer.width, stateData.graphicsContainer.height);						   //vytvoření textury o velikosti okna
	renderSprite.setTexture(renderTexture.getTexture());																   //nastavení textury pro sprite
	renderSprite.setTextureRect(sf::IntRect(0, 0, stateData.graphicsContainer.width, stateData.graphicsContainer.height)); //nastavení velikosti textury ve spritu
}

void EditorState::InitGUI()
{
	sideBar.setSize(sf::Vector2f(GUI::PeToPiX(5.2f, stateData.graphicsContainer.width), stateData.window->getSize().y));
	sideBar.setPosition(GUI::PeToPiX(0.f, stateData.graphicsContainer.width), GUI::PeToPiY(0.f, stateData.graphicsContainer.height));
	sideBar.setFillColor(sf::Color(95, 95, 95, 100));
	sideBar.setOutlineColor(sf::Color(255, 255, 255, 155));
	sideBar.setOutlineThickness(3.f);

	mapBorders.setSize(sf::Vector2f(map.getMapGridSize().x * stateData.gridSize, map.getMapGridSize().y * stateData.gridSize));
	mapBorders.setPosition(0.f, 0.f);
	mapBorders.setFillColor(sf::Color::Transparent);
	mapBorders.setOutlineColor(sf::Color(255, 255, 255, 155));
	mapBorders.setOutlineThickness(4.f);

	pauseMenu.AddButton("RESUME_BTN", "Resume", 6.9f, sf::Vector2f(10.4f, 7.5f), font);
	pauseMenu.AddButton("SETTINGS_BTN", "Settings", 20.7f, sf::Vector2f(10.4f, 7.5f), font);
	pauseMenu.AddButton("QUIT_BTN", "Quit", 34.5f, sf::Vector2f(10.4f, 7.5f), font);

	std::vector<std::string> maps;

	for (auto& a : resourceman->GetMaps())
	{
		maps.push_back(a.first);
	}

	pauseMenu.AddButton("SAVE_MAP", "Save Map", 48.3f, sf::Vector2f(10.4f, 7.5f), font);
	pauseMenu.AddButton("LOAD_MAP", "Load Map", 62.1f, sf::Vector2f(10.4f, 7.5f), font);
	pauseMenu.AddDropDown("MAPS", 75.9f, sf::Vector2f(10.4f, 7.5f), font, maps);

	texSelectors["TILES_TEXSELECTOR"] = std::unique_ptr<GUI::TextureSelector>(new GUI::TextureSelector(0.f, 0.f, sf::Vector2f(5.2f, 9.2f), "Tiles", 256.f, 352.f, 32.f, sf::Vector2f(32.f, 32.f), font, map.GetTileSheet(), stateData.graphicsContainer.GetSize()));
	texSelectors["OBJECT_TEXSELECTOR"] = std::unique_ptr<GUI::TextureSelector>(new GUI::TextureSelector(0.f, 9.2f, sf::Vector2f(5.2f, 9.2f), "Objects", 32.f, 704.f, 32.f, sf::Vector2f(32.f, 32.f), font, map.GetObjectSheet(), stateData.graphicsContainer.GetSize()));
	texSelectors["PORTAL_TEXSELECTOR"] = std::unique_ptr<GUI::TextureSelector>(new GUI::TextureSelector(0.f, 18.4f, sf::Vector2f(5.2f, 9.2f), "Portals", 32.f, 64.f, 32.f, sf::Vector2f(32.f, 32.f), font, map.GetPortalSheet(), stateData.graphicsContainer.GetSize()));
	texSelectors["ENEMY_TEXSELECTOR"] = std::unique_ptr<GUI::TextureSelector>(new GUI::TextureSelector(0.f, 27.5f, sf::Vector2f(5.2f, 9.2f), "Enemies", 32.f, 128.f, 32.f, sf::Vector2f(32.f, 32.f), font, map.GetEnemySheet(), stateData.graphicsContainer.GetSize()));
	texSelectors["NPC_TEXSELECTOR"] = std::unique_ptr<GUI::TextureSelector>(new GUI::TextureSelector(0.f, 36.7f, sf::Vector2f(5.2f, 9.2f), "NPCs", 32.f, 224.f, 32.f, sf::Vector2f(32.f, 32.f), font, map.GetNPCSheet(), stateData.graphicsContainer.GetSize()));

	info.setPosition(GUI::PeToPiX(80.f, stateData.graphicsContainer.width), GUI::PeToPiY(80.f, stateData.graphicsContainer.height));
	info.setFont(font);
	info.setCharacterSize(GUI::CaclCharSize(stateData.graphicsContainer.width, stateData.graphicsContainer.height));

	selector.setSize(sf::Vector2f((float)stateData.gridSize, (float)stateData.gridSize));
	selector.setOutlineColor(sf::Color(255, 255, 255));
	selector.setOutlineThickness(2.f);
	selector.setTexture(&map.GetTileSheet());
	selector.setTextureRect(selectedTile);
}

void EditorState::InitState()
{
	InitKeybinds();
	InitFont();
	InitView();
	InitDeferredRender();
	map.InitMap();
	InitGUI();
}

void EditorState::AfterInit()
{
	sideBar.setSize(sf::Vector2f(GUI::PeToPiX(5.2f, stateData.graphicsContainer.width), stateData.window->getSize().y));
	sideBar.setPosition(GUI::PeToPiX(0.f, stateData.graphicsContainer.width), GUI::PeToPiY(0.f, stateData.graphicsContainer.height));

	for (auto& a : texSelectors)
	{
		a.second->AferInit(stateData.graphicsContainer.GetSize());
	}

	info.setCharacterSize(GUI::CaclCharSize(stateData.graphicsContainer.GetSize()));
	info.setPosition(GUI::PeToPiX(80.f, stateData.graphicsContainer.width), GUI::PeToPiY(80.f, stateData.graphicsContainer.height));

	pauseMenu.AfterInit(stateData.graphicsContainer.GetSize());

	InitView();
	InitDeferredRender();
}

void EditorState::CheckQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("EXIT"))) && GetKeyTime() == true)
	{
		Pause();
	}
}

void EditorState::InputGUI()
{
	CheckQuit();
}

void EditorState::InputModes()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("ENABLE_SINGLE_LAYER_MODE"))) && GetKeyTime() == true)
	{
		singleLayerMode = !singleLayerMode; //enables/disables single layer mode
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("ENABLE_COLLISION_FOR_TILES"))) && GetKeyTime() == true)
	{
		enableCollision = !enableCollision;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("ENABLE_TILE_REPLACING"))) && GetKeyTime() == true)
	{
		tileReplace = !tileReplace;
	}
}

void EditorState::InputPlayer(float dt)
{
	int msDelta = GetMouseWheelDelta(); // gets delta value of mouse wheel

	if (msDelta < 0)
	{
		view.zoom(1.25); //zoom in
	}
	if (msDelta > 0)
	{
		view.zoom(0.75); //zoom out
	}

	InputModes();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_UP_LAYER"))) && GetKeyTime() == true)
	{
		if (selLayer < (map.GetNOLayer() - 1))
		{
			selLayer++; //increases one layer
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_DOWN_LAYER"))) && GetKeyTime() == true)
	{
		if (selLayer > 0)
		{
			selLayer--; //decreases layer
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_CAMERA_UP"))))
	{
		cameraPos += sf::Vector2f(cameraPos.x, cameraPos.y + (-cameraSpeed * dt));
		view.move(0.f, -cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_CAMERA_DOWN"))))
	{
		cameraPos += sf::Vector2f(cameraPos.x, cameraPos.y + (cameraSpeed * dt));
		view.move(0.f, cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_CAMERA_LEFT"))))
	{
		cameraPos += sf::Vector2f(cameraPos.x + (-cameraSpeed * dt), cameraPos.y);
		view.move(-cameraSpeed * dt, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_CAMERA_RIGHT"))))
	{
		cameraPos += sf::Vector2f(cameraPos.x + (cameraSpeed * dt), cameraPos.y);
		view.move(cameraSpeed * dt, 0.f);
	}

	if (sideBar.getGlobalBounds().contains((sf::Vector2f)mousePosWindow) == false)
	{ //pokud není myška nad GUI elementama
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{ //levé tlačitko pro položení dlaždice
			switch (tileTypeSelected)
			{ //podle toho, jaký typ dlaždice je vybrán, tak se zvolí dana funkce k položení dlaždice
				case tileTypes::tile:
					map.PlaceTile(mousePosGrid.x, mousePosGrid.y, tileReplace, selLayer, selectedTile, enableCollision);
					break;
				case tileTypes::object:
					map.PlaceObject(texSelectors.at("OBJECT_TEXSELECTOR")->GetSelectedID(), mousePosGrid.x, mousePosGrid.y, tileReplace, selLayer, enableCollision);
					break;
				case tileTypes::portal:
					map.PlacePortal(texSelectors.at("PORTAL_TEXSELECTOR")->GetSelectedID(), mousePosGrid.x, mousePosGrid.y, tileReplace, selLayer, enableCollision);
					break;
				case tileTypes::enemy:
					map.PlaceEnemy(texSelectors.at("ENEMY_TEXSELECTOR")->GetSelectedID(), mousePosGrid.x, mousePosGrid.y, tileReplace, selLayer, enableCollision);
					break;
				case tileTypes::npc:
					map.PlaceNPC(texSelectors.at("NPC_TEXSELECTOR")->GetSelectedID(), mousePosGrid.x, mousePosGrid.y, tileReplace, selLayer, enableCollision);
				default:
					break;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{ //pravé tlačítko pro smazání dlaždice
			map.RemoveTile(mousePosGrid.x, mousePosGrid.y, selLayer);
		}
	}
}

void EditorState::Input()
{
}

void EditorState::UpdateInfoText()
{
	info.setString("Layer: " + std::to_string(selLayer) + "/" + std::to_string(map.GetNOLayer() - 1) + '\n'
		+ "Single Layer Mode: " + std::to_string(singleLayerMode) + '\n'
		+ "Collision: " + std::to_string(enableCollision) + '\n'
		+ "Tile replacing:" + std::to_string(tileReplace) + '\n');
}

void EditorState::UpdatePauseMenu(float dt)
{
	pauseMenu.Update(mousePosWindow, dt);

	if (pauseMenu.GetButton("RESUME_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		Pause();
	}
	if (pauseMenu.GetButton("QUIT_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		wantsToEnd = true;
	}
	if (pauseMenu.GetButton("SETTINGS_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		stateData.states->push(new SettingsState(stateData));
	}
	if (pauseMenu.GetButton("SAVE_MAP")->GetState() == btnStates::BTN_PRESSED)
	{
		map.SaveCurrentMap(resourceman->SaveDataAt(stateData.save, pauseMenu.GetDropDown("MAPS").GetActiveText()));
	}

	if (pauseMenu.GetButton("LOAD_MAP")->GetState() == btnStates::BTN_PRESSED)
	{
		map.LoadMap(resourceman->SaveDataAt(stateData.save, pauseMenu.GetDropDown("MAPS").GetActiveText()));
		mapBorders.setSize(sf::Vector2f(map.getMapGridSize().x * stateData.gridSize, map.getMapGridSize().y * stateData.gridSize));
	}
}

void EditorState::UpdateGUI(float dt)
{
	for (auto& a : texSelectors)
	{
		a.second->Update(dt, mousePosWindow);
	}
	for (auto& a : btns)
	{
		a.second->Update(mousePosWindow);
	}
}

void EditorState::UpdateSelector()
{
	//nastavení pozice selectoru pozice myši
	this->selector.setPosition(mousePosGrid.x * stateData.gridSize, mousePosGrid.y * stateData.gridSize);
	switch (tileTypeSelected)
	{ //podle toho, jaký typ dlaždice je vybrán, tak se selectoru nastaví daná textura
		case tileTypes::tile:
			this->selector.setTexture(&map.GetTileSheet());
			break;
		case tileTypes::object:
			this->selector.setTexture(&map.GetObjectSheet());
			break;
		case tileTypes::portal:
			this->selector.setTexture(&map.GetPortalSheet());
			break;
		case tileTypes::enemy:
			this->selector.setTexture(&map.GetEnemySheet());
			break;
		case tileTypes::npc:
			this->selector.setTexture(&map.GetNPCSheet());
			break;
		default:
			break;
	}

	this->selector.setTextureRect(selectedTile); // nastavení oblasti v textuře, která se bude zobrazovat
}

void EditorState::Update(float dt)
{

	UpdateKeyTime(dt);	 //aktualizace keytime
	UpdateMosPos(&view); //aktualizace pozice myši
	InputGUI();			 // zachytává eventy pro GUI prvky

	if (!paused)
	{						   //pokud hra není pauznutá
		InputPlayer(dt);	   // zachytávání eventů pro hráče
		CheckInputSelectors(); //aktualizace selectorů dlaždic

		UpdateSelector(); //aktualizuje pozici ukazele
		UpdateGUI(dt);	  //aktualizace GUI prvků
		UpdateInfoText(); //aktualizace informativního textu
	}
	else
	{
		UpdatePauseMenu(dt); // když je hra pozastavena, aktualizace GUI elementů v menu
	}
}

void EditorState::DisplayGUI()
{
	renderTexture.draw(sideBar);

	for (auto& a : texSelectors)
	{ //renders all texture selectors
		a.second->Display(renderTexture);
	}

	renderTexture.draw(info); //renders text

	for (auto& a : btns)
	{ // renders all btns
		a.second->Display(renderTexture);
	}
}

void EditorState::Display(sf::RenderTarget* target)
{
	if (!target)
		target = stateData.window;

	renderTexture.clear();

	renderTexture.setView(view);

	renderTexture.draw(mapBorders);

	if (singleLayerMode == true)
	{ //if single layer mode enabled, displays only one layer
		map.DisplayDebug(renderTexture, selLayer, cameraPos);
	}
	else
	{
		map.DisplayDebug(renderTexture, cameraPos); //renders map
	}

	renderTexture.draw(selector); //renders selector

	renderTexture.setView(renderTexture.getDefaultView());
	if (IsPaused()) //pause menu render
	{
		pauseMenu.Display(renderTexture);
	}

	DisplayGUI(); // renders gui elements

	//final render
	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	target->draw(renderSprite);
}

void EditorState::EndState()
{
}

void EditorState::CheckInputSelectors()
{
	if (texSelectors.at("TILES_TEXSELECTOR")->WasChanged())
	{
		selectedTile = texSelectors.at("TILES_TEXSELECTOR")->GetSelectedTexture();
		tileTypeSelected = tileTypes::tile;
		for (auto& a : texSelectors)
		{
			a.second->hidden = true;
		}
		texSelectors.at("TILES_TEXSELECTOR")->hidden = false;
	}
	if (texSelectors.at("OBJECT_TEXSELECTOR")->WasChanged())
	{
		selectedTile = texSelectors.at("OBJECT_TEXSELECTOR")->GetSelectedTexture();
		tileTypeSelected = tileTypes::object;
		for (auto& a : texSelectors)
		{
			a.second->hidden = true;
		}
		texSelectors.at("OBJECT_TEXSELECTOR")->hidden = false;
	}
	if (texSelectors.at("PORTAL_TEXSELECTOR")->WasChanged())
	{
		selectedTile = texSelectors.at("PORTAL_TEXSELECTOR")->GetSelectedTexture();
		tileTypeSelected = tileTypes::portal;
		for (auto& a : texSelectors)
		{
			a.second->hidden = true;
		}
		texSelectors.at("PORTAL_TEXSELECTOR")->hidden = false;
	}
	if (texSelectors.at("ENEMY_TEXSELECTOR")->WasChanged())
	{
		selectedTile = texSelectors.at("ENEMY_TEXSELECTOR")->GetSelectedTexture();
		tileTypeSelected = tileTypes::enemy;
		for (auto& a : texSelectors)
		{
			a.second->hidden = true;
		}
		texSelectors.at("ENEMY_TEXSELECTOR")->hidden = false;
	}
	if (texSelectors.at("NPC_TEXSELECTOR")->WasChanged())
	{
		selectedTile = texSelectors.at("NPC_TEXSELECTOR")->GetSelectedTexture();
		tileTypeSelected = tileTypes::npc;
		for (auto& a : texSelectors)
		{
			a.second->hidden = true;
		}
		texSelectors.at("NPC_TEXSELECTOR")->hidden = false;
	}
}
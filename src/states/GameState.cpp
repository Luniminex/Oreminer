#include <states/GameState.hpp>

//constructors and destructors

GameState::GameState(StateData& stateData) :
	State(stateData),
	results(),
	pausemenu(stateData.graphicsContainer.GetSize()),
	tileMap(sf::Vector2u(1, 1), stateData.gridSize, 1)
{
	InitState();
}

GameState::~GameState()
{
	std::cout << "Deleted GameState" << std::endl;
}

void GameState::InitFonts()
{
	if (!font.loadFromFile(resourceman->ConfigAt("ROBOTOFONT")))
	{
		std::cout << "Could not load font in gamestate/initfonts" << std::endl;
	}
}

//public functions
void GameState::InitTextures()
{
	sf::Texture temp;
	if (!temp.loadFromFile(resourceman->TextureAt("PLAYERADVENTURESHEET")))
	{
		std::cout << "GAMESTATE:: FAILED TO LOAD PLAYER TEXTURE" << std::endl;
	}
	textures["PlayerTexture"] = temp;
	if (!temp.loadFromFile(resourceman->TextureAt("ITEMSHEET")))
	{
		std::cout << "GAMESTATE:: FAILED TO LOAD ITEM TEXTURE" << std::endl;
	}
	textures["ITEM_TEXTURE"] = temp;
}

void GameState::InitKeybinds()
{
	std::ifstream ifstr(resourceman->SaveDataAt(stateData.save, "GAMEKEYBINDS"));
	std::string action;
	std::string key;

	while (ifstr >> action >> key)
	{
		keybinds[action] = stateData.supportedKeys->at(key); //sets action to specific key
	}
	ifstr.close();
}

void GameState::InitView()
{

	view.setSize(stateData.graphicsContainer.GetSize());

	view.setCenter(stateData.window->getSize().x / 2.f, stateData.window->getSize().y / 2.f);
	view.zoom(0.7);
}

void GameState::InitDeferredRendering()
{
	renderTexture.create(stateData.graphicsContainer.width, stateData.graphicsContainer.height);

	renderSprite.setTexture(renderTexture.getTexture());
	renderSprite.setTextureRect(sf::IntRect(0, 0, stateData.graphicsContainer.width, stateData.graphicsContainer.height));
}

void GameState::InitGUI()
{
	pausemenu.AddButton("RESUME_BTN", "Resume", 13.8f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("SETTINGS_BTN", "Settings", 27.7f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("SAVE_BTN", "Save and Quit", 41.6f, sf::Vector2f(10.4f, 7.5f), font);
	pausemenu.AddButton("QUIT_BTN", "Quit", 55.2f, sf::Vector2f(10.4f, 7.5f), font);
}

void GameState::InitItems()
{
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(resourceman->DataAt("ITEMDATA"));
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("items")->first_node("item");

	const char* temp = "";

	std::string name = "";
	std::string description = "";
	bool stackable = false;
	unsigned int maxStack = 0;
	int ID = 0;
	int type = ItemType::material;
	sf::Vector2f texCoords = { 0, 0 };

	attributes attrs; //temp attributes to store attributes from items

	attrs.ResetAll();

	enum strings
	{
		Eid,
		Ename,
		Estackable,
		Estack,
		EtexturePositionX,
		EtexturePositionY,
		Edescription,
		Etype,
		/*attributes*/
		EminingPower,
		Ehealth,
		EmaxHealth,
		EhealthRegen,
		Emana,
		EmaxMana,
		EmanaRegen,
		Estamina,
		EmaxStamina,
		EstaminaRegen,
		Estrenght,
		EcritDamage,
		EcritChance,
		EdodgeChance,
		Earmour
	};

	std::map<std::string, int> strMap;

	strMap["id"] = strings::Eid;
	strMap["name"] = strings::Ename;
	strMap["description"] = strings::Edescription;
	strMap["stackable"] = strings::Estackable;
	strMap["stack"] = strings::Estack;
	strMap["texturePositionX"] = strings::EtexturePositionX;
	strMap["texturePositionY"] = strings::EtexturePositionY;
	strMap["description"] = strings::Edescription;
	strMap["type"] = strings::Etype;
	/*items*/
	strMap["miningPower"] = strings::EminingPower;
	strMap["health"] = strings::Ehealth;
	strMap["maxhealth"] = strings::EmaxHealth;
	strMap["healthregen"] = strings::EhealthRegen;
	strMap["mana"] = strings::Emana;
	strMap["maxmana"] = strings::EmaxMana;
	strMap["manaregen"] = strings::EmanaRegen;
	strMap["stamina"] = strings::Estamina;
	strMap["maxstamina"] = strings::EmaxStamina;
	strMap["staminaregen"] = strings::EstaminaRegen;
	strMap["strenght"] = strings::Estrenght;
	strMap["critdamage"] = strings::EcritDamage;
	strMap["critchance"] = strings::EcritChance;
	strMap["dodgechance"] = strings::EdodgeChance;
	strMap["armour"] = strings::Earmour;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			switch (strMap.at(child->name()))
			{
				case strings::Eid:
					temp = child->value();
					ID = std::stoi(temp);
					break;
				case strings::Etype:
					temp = child->value();
					type = std::stoi(temp);
					break;
				case strings::Ename:
					name = child->value();
					break;
				case strings::Edescription:
					description = child->value();
					break;
				case strings::Estackable:
					temp = child->value();
					stackable = std::stoi(temp);
					break;
				case strings::Estack:
					temp = child->value();
					maxStack = std::stoul(temp);
					break;
				case strings::EtexturePositionX:
					temp = child->value();
					texCoords.x = std::stoi(temp);
					break;
				case strings::EtexturePositionY:
					temp = child->value();
					texCoords.y = std::stoi(temp);
					break;
				case strings::EminingPower:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::miningPower) = std::stoi(temp);
					break;
				case strings::Ehealth:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::health) = std::stoi(temp);
					break;
				case strings::EmaxHealth:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::maxHealth) = std::stoi(temp);
					break;
				case strings::EhealthRegen:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::healthRegen) = std::stoi(temp);
					break;
				case strings::Emana:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::mana) = std::stoi(temp);
					break;
				case strings::EmaxMana:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::maxMana) = std::stoi(temp);
					break;
				case strings::EmanaRegen:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::manaRegen) = std::stoi(temp);
					break;
				case strings::Estamina:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::stamina) = std::stoi(temp);
					break;
				case strings::EmaxStamina:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::maxStamina) = std::stoi(temp);
					break;
				case strings::EstaminaRegen:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::staminaRegen) = std::stoi(temp);
					break;
				case strings::Estrenght:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::strenght) = std::stoi(temp);
					break;
				case strings::EcritDamage:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::critDamage) = std::stoi(temp);
					break;
				case strings::EcritChance:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::critChance) = std::stoi(temp);
					break;
				case strings::EdodgeChance:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::dodgeChance) = std::stoi(temp);
					break;
				case strings::Earmour:
					temp = child->value();
					attrs.atrTypes.at(ATRTYPES::armour) = std::stoi(temp);
					break;
				default:
					break;
			}
		}
		itemTemplates[ID] = std::unique_ptr<Item>(new Item(ID, type, name, description, stackable, maxStack, 1, texCoords, attrs));
		attrs.ResetAll();
	}

	doc.clear();
	delete node;
	delete temp;

	Quester::LoadItem(&itemTemplates);

	std::cout << "LOADED ITEMS" << std::endl;
}

void GameState::InitRecipes()
{
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(resourceman->DataAt("RECIPEDATA"));
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("interfaces")->first_node("interface");

	const char* temp = nullptr;

	int craftinterfaceID = 0;
	std::vector<Recipe> tempRecipes;
	std::vector<int> amounts;
	std::vector<int> ingredientsID;
	int result = 0;

	enum strings
	{
		Ecraftid,
		EtempRecipes,
		EingredientID,
		Eamount,
		EresultID
	};

	std::map<std::string, int> strMap;

	strMap["id"] = strings::Ecraftid;
	strMap["recipe"] = strings::EtempRecipes;
	strMap["ingredient"] = strings::EingredientID;
	strMap["amount"] = strings::Eamount;
	strMap["result"] = strings::EresultID;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			switch (strMap.at(child->name()))
			{
				case strings::Ecraftid:
					temp = child->value();
					craftinterfaceID = std::stoi(temp);
					break;
				case strings::EtempRecipes:
					for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
					{
						switch (strMap.at(grandchild->name()))
						{
							case strings::EingredientID:
								temp = grandchild->value();
								ingredientsID.push_back(std::stoi(temp));
								break;
							case strings::Eamount:
								temp = grandchild->value();
								amounts.push_back(std::stoi(temp));
								break;
							case strings::EresultID:
								temp = grandchild->value();
								result = std::stoi(temp);
								break;
							default:
								break;
						}
					}
					tempRecipes.push_back(Recipe(ingredientsID, amounts, result));
					ingredientsID.clear();
					amounts.clear();
					break;
				default:
					break;
			}
		}
		recipes[craftinterfaceID] = tempRecipes;
		tempRecipes.clear();
		ingredientsID.clear();
		amounts.clear();
	}
	doc.clear();
	delete node;
	delete temp;
}

void GameState::InitState()
{
	InitFonts();
	InitTextures();
	InitKeybinds();
	InitView();
	InitDeferredRendering();
	InitGUI();
	InitItems();
	InitRecipes();

	RegisterObservers();

	LoadSave(stateData.save);

	Quester::LoadPlayer(player);
	Quester::LoadQuests(stateData.save);
	Statistics::LoadStatistics(stateData.save);

	NotifyObservers(SubjectActions::ENTER_GAMESTATE);

	std::cout << "Started GameState" << std::endl;
}

void GameState::AfterInit()
{
	NotifyObservers(SubjectActions::ENTER_GAMESTATE);
	pausemenu.AfterInit(stateData.graphicsContainer.GetSize());
	player->AfterInitGUI(stateData.graphicsContainer.GetSize());
	InitView();
	InitDeferredRendering();
}

void GameState::CheckQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("PAUSE"))) && GetKeyTime() == true)
	{
		Pause();
	}
}

void GameState::Input()
{
	CheckQuit();
}

void GameState::UpdateButtons(float dt)
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
		player->SaveItems(resourceman->SaveDataAt(stateData.save, "PLAYERITEMS"));
		player->SaveAttributes(resourceman->SaveDataAt(stateData.save, "PLAYERATRRIBUTES"));
		tileMap.SaveCurrentMap(resourceman->SaveDataAt(stateData.save, "STARTINGMAP"));
		wantsToEnd = true;
	}

	if (pausemenu.GetButton("QUIT_BTN")->GetState() == btnStates::BTN_PRESSED)
	{
		wantsToEnd = true;
	}
}

void GameState::Update(float dt)
{
	UpdateKeyTime(dt);
	UpdateMosPos(&view);
	Input();
	if (!IsPaused())
	{ //update unpaused
		player->Update(dt, mousePosWindow);
		UpdateMap(dt);
		CheckFightEnded();
		view.setCenter(player->GetPosition());
	}
	else
	{ //update paused
		AudioPlayer::PauseAllOnLoop(true);
		UpdateButtons(dt);
	}
}

void GameState::Display(sf::RenderTarget* target)
{
	if (!target)
	{ // if no target specified use window from constructor
		target = stateData.window;
	}

	renderTexture.clear();		 // clears render texture
	renderTexture.setView(view); //sets view

	for (unsigned int layer = 0; layer < tileMap.GetNOLayer(); ++layer)
	{

		tileMap.Display(renderTexture, player->GetPosition(), layer, view.getSize());
		if (layer == player->GetZ())
		{
			player->Display(renderTexture);
		}
	}

	renderTexture.setView(renderTexture.getDefaultView());
	player->DisplayGUI(renderTexture);

	if (IsPaused())
	{ //if paused, draw pause menu
		pausemenu.Display(renderTexture);
	}

	//rendering to screen
	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	target->draw(renderSprite);
}

void GameState::EndState()
{
	delete player;
}

//private
void GameState::CheckFightEnded()
{
	if (results.FightEnded())
	{
		if (results.PlayerWon())
		{

			player->GiveExp(results.GetExperienceDropped());
			for (auto& a : results.GetItemsDropped())
			{
				player->GiveItem(*itemTemplates.at(a.first), a.second);
				NotifyObservers(SubjectActions::GATHER_ITEM, a.first);
			}
			results.GetItemsDropped().clear();
			tileMap.GetEnemySensed().SetDead(true);
			NotifyObservers(SubjectActions::KILL_ENEMY, tileMap.GetEnemySensed().GetID());
			results.SetFightEnded(false);
		}
		else
		{
			LoadSave(stateData.save);
			results.SetFightEnded(false);
		}
	}
}

void GameState::UpdateMap(float dt)
{
	tileMap.CheckCollision(*player, dt);
	if (tileMap.UpdateMap(dt, *player) == 1)
	{
		player->SetLastMapPosition(player->GetPosition());
		stateData.states->push(new FightState(stateData, *player, tileMap.GetEnemySensedData(), itemTemplates, textures.at("ITEM_TEXTURE"), results, tileMap.GetMapType()));
	}
}

void GameState::LoadSave(int save)
{
	tileMap.InitMap(&itemTemplates);
	tileMap.LoadMap(resourceman->SaveDataAt(save, "STARTINGMAP"));
	player = new Player(&textures["PlayerTexture"], &keybinds, font, textures.at("ITEM_TEXTURE"), recipes, itemTemplates, stateData.graphicsContainer.GetSize());
	player->LoadItems(resourceman->SaveDataAt(save, "PLAYERITEMS"), itemTemplates);
	player->LoadAttributes(resourceman->SaveDataAt(save, "PLAYERATRRIBUTES"));
	player->SetPosition(tileMap.GetSpawnPointCoordinates().x, tileMap.GetSpawnPointCoordinates().y);
}

void GameState::RegisterObservers()
{
	RegisterObserver(std::make_shared<Quester>());
	RegisterObserver(std::make_shared<Statistics>());
	RegisterObserver(std::make_shared<AudioPlayer>());
}
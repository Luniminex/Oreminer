#include "map/TileMap.hpp"

//constructors / destructors
Tilemap::Tilemap(sf::Vector2u mapSize, int gridSize, unsigned int layerCount, std::map<int, std::unique_ptr<Item>>* itemTemplates)
{

	RegisterObserver(std::make_shared<Quester>());
	RegisterObserver(std::make_shared<Statistics>());
	RegisterObserver(std::make_shared<AudioPlayer>());

	this->mapSize = mapSize;
	this->gridSize = gridSize;
	this->layerCount = layerCount;
	this->itemTemplates = itemTemplates;

	fromX = 0;
	fromY = 0;
	toX = 0;
	toY = 0;

	collisionIndicator.setSize(sf::Vector2f(gridSize, gridSize));
	collisionIndicator.setFillColor(sf::Color(255, 0, 0, 55));

	enemySensed = nullptr;
}

Tilemap::~Tilemap()
{

	for (auto& rows : map) // vyčistí mapu a objekty
	{
		for (auto& colls : rows)
		{
			for (auto& layer : colls)
			{
				delete layer;
			}
		}
	}
	if (enemySensed != nullptr)
	{
		delete enemySensed;
	}
}

sf::Texture& Tilemap::GetTileSheet()
{
	return tileSheet;
}

sf::Texture& Tilemap::GetObjectSheet()
{
	return objectSheet;
}

sf::Texture& Tilemap::GetPortalSheet()
{
	return portalSheet;
}

sf::Texture& Tilemap::GetEnemySheet()
{
	return enemySheet;
}

sf::Texture& Tilemap::GetNPCSheet()
{
	return npcSheet;
}

unsigned int Tilemap::GetNOLayer()
{
	return layerCount;
}

sf::Vector2i Tilemap::getMapGridSize()
{
	return static_cast<sf::Vector2i>(mapSize);
}

std::map<int, std::unique_ptr<Item>>& Tilemap::GetItemTemplates()
{
	return *itemTemplates;
}

sf::Vector2f Tilemap::GetSpawnPointCoordinates()
{
	return sf::Vector2f(spawnPoint.x * gridSize, spawnPoint.y * gridSize);
}

EnemyObject& Tilemap::GetEnemySensed()
{
	return *enemySensed;
}

EnemyData Tilemap::GetEnemySensedData()
{
	return *enemyTemplates.at(enemySensed->GetID());
}

int Tilemap::GetMapType()
{
	return mapType;
}

//public functions

void Tilemap::InitMap(std::map<int, std::unique_ptr<Item>>* itemTemplates)
{

	this->itemTemplates = itemTemplates;

	map.resize(mapSize.x, std::vector<std::vector<Tile*>>()); // resizes rows
	for (auto& coll : map)
	{
		coll.resize(mapSize.y, std::vector<Tile*>()); // resizes collumns
		for (auto& layers : coll)
		{
			layers.resize(layerCount, nullptr); // resizes layers
		}
	}

	LoadTileSheet(resourceman->TextureAt("MAPGROUNDTSHEET"), resourceman->TextureAt("MAPOBJECTSHEET"), resourceman->DataAt("OBJECTDATA"), resourceman->TextureAt("MAPPORTALSHEET"), resourceman->DataAt("PORTALDATA"), resourceman->TextureAt("ENEMYSHEET"), resourceman->DataAt("ENEMYDATA"), resourceman->TextureAt("NPCSHEET"), resourceman->DataAt("NPCDATA"));
}

void Tilemap::LoadTileSheet(const std::string& tileTexture, const std::string& objectTexture, const std::string& objectData, const std::string& portalTexture, const std::string& portalPaths, const std::string& enemyTexture, const std::string& enemyData, const std::string& npcTexture, const std::string& npcData)
{
	if (!tileSheet.loadFromFile(tileTexture))
	{
		std::cout << "TILEMAP::COULD NOT LOAD TILESHEET" << std::endl;
	}

	LoadObjectSheet(objectTexture); //objects
	LoadObjectData(objectData);

	LoadPortalSheet(portalTexture); //portals
	LoadMapPaths(portalPaths);

	LoadEnemySheet(enemyTexture); //enemies
	LoadEnemyData(enemyData);

	LoadNPCSheet(npcTexture);
	LoadNPCData(npcData);
}

void Tilemap::PlaceTile(unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, const sf::IntRect& tileRect, bool isCollision)
{
	if (gridX < mapSize.x && gridY < mapSize.y)
	{ //zjistí, jestli se snaží položit dlaždici uvnitřohraničení mapy
		if (map.at(gridX).at(gridY).at(layer) == nullptr || canReplace == true)
		{ //pokud je na pozici místo, nebo je zapnutý mód pro automatické nahrazování dlaždic
			map.at(gridX).at(gridY).at(layer) = new Tile(gridX, gridY, gridSize, tileSheet, tileRect, isCollision);
		}
		else
		{ //na místě už je nějaká dlaždice
			std::cout << "Tile already exists at:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{ //snaží se položit dlaždici mimo hranice mapy
		std::cout << "Trying to place a tile out of boundaries!" << std::endl;
	}
}

void Tilemap::PlaceObject(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision)
{ //fks map boundaries
	if (gridX < mapSize.x && gridY < mapSize.y)
	{
		if (map.at(gridX).at(gridY).at(layer) == nullptr || canReplace == true)
		{
			objects[std::pair(gridX, std::pair(gridY, layer))] = new Object(id, gridX, gridY, gridSize, objectSheet, sf::IntRect(objectTemplates.at(id)->textureRect.x, objectTemplates.at(id)->textureRect.y, 32, 32), isCollision, objectTemplates.at(id)->name, objectTemplates.at(id)->hardness, objectTemplates.at(id)->resourceType, objectTemplates.at(id)->resourceRemaining, objectTemplates.at(id)->destroyedTex);
			//objects.push_back(new Object(id, gridX, gridY, gridSize, objectSheet, sf::IntRect(objectTemplates.at(id)->textureRect.x, objectTemplates.at(id)->textureRect.y, 32, 32), isCollision, objectTemplates.at(id)->name, objectTemplates.at(id)->resourceType, objectTemplates.at(id)->resourceRemaining, objectTemplates.at(id)->destroyedTex));
			map.at(gridX).at(gridY).at(layer) = objects.at(std::pair(gridX, std::pair(gridY, layer)));
		}
		else
		{
			std::cout << "Tile already exists at:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{
		std::cout << "Trying to place a object out of boundaries!" << std::endl;
	}
}

void Tilemap::PlacePortal(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision)
{
	std::cout << "Placing portal with ID: " << id << std::endl;
	if (gridX < mapSize.x && gridY < mapSize.y)
	{
		if (map.at(gridX).at(gridY).at(layer) == nullptr || canReplace == true)
		{
			portals[std::pair(gridX, std::pair(gridY, layer))] = new Portal(gridX, gridY, gridSize, portalSheet, sf::IntRect(portalTemplates.at(id)->texCoords.x, portalTemplates.at(id)->texCoords.y, gridSize, gridSize), isCollision, id, 1);
			map.at(gridX).at(gridY).at(layer) = portals.at(std::pair(gridX, std::pair(gridY, layer)));
		}
		else
		{
			std::cout << "Tile already exists at:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{
		std::cout << "Trying to place a object out of boundaries!" << std::endl;
	}
}

void Tilemap::PlaceEnemy(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision)
{
	std::cout << "Placing enemy with ID: " << id << std::endl;
	if (gridX < mapSize.x && gridY < mapSize.y)
	{
		if (map.at(gridX).at(gridY).at(layer) == nullptr || canReplace == true)
		{
			enemies[std::pair(gridX, std::pair(gridY, layer))] = new EnemyObject(id, 0, gridX, gridY, gridSize, enemySheet, sf::IntRect(enemyTemplates.at(id)->startCoords.x, enemyTemplates.at(id)->startCoords.y, gridSize, gridSize), isCollision, enemyTemplates.at(id)->frames);
			map.at(gridX).at(gridY).at(layer) = enemies.at(std::pair(gridX, std::pair(gridY, layer)));
		}
		else
		{
			std::cout << "Tile already exists at:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{
		std::cout << "Trying to place a enemy out of boundaries!" << std::endl;
	}
}

void Tilemap::PlaceNPC(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision)
{
	std::cout << "Placing npc with ID: " << id << std::endl;
	if (gridX < mapSize.x && gridY < mapSize.y)
	{
		if (map.at(gridX).at(gridY).at(layer) == nullptr || canReplace == true)
		{
			npcs[std::pair(gridX, std::pair(gridY, layer))] = new NPC(id, 0, npcTemplates.at(id)->hasCraftingInterface, gridX, gridY, gridSize, npcSheet, sf::IntRect(npcTemplates.at(id)->startCoords.x, npcTemplates.at(id)->startCoords.y, gridSize, gridSize), isCollision, npcTemplates.at(id)->frames);
			map.at(gridX).at(gridY).at(layer) = npcs.at(std::pair(gridX, std::pair(gridY, layer)));
		}
		else
		{
			std::cout << "Tile already exists at:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{
		std::cout << "Trying to place a npc out of boundaries!" << std::endl;
	}
}

void Tilemap::RemoveTile(unsigned int gridX, unsigned int gridY, unsigned int layer)
{

	//checks map boundaries
	if (gridX < mapSize.x && gridY < mapSize.y)
	{

		if (map.at(gridX).at(gridY).at(layer) != nullptr)
		{

			switch (map.at(gridX).at(gridY).at(layer)->GetType())
			{
				case tileTypes::tile:
					delete map.at(gridX).at(gridY).at(layer);
					map.at(gridX).at(gridY).at(layer) = nullptr;
					break;
				case tileTypes::object:
					objects.erase(std::pair(gridX, std::pair(gridY, layer)));
					delete map.at(gridX).at(gridY).at(layer);
					map.at(gridX).at(gridY).at(layer) = nullptr;
					break;
				case tileTypes::portal:
					portals.erase(std::pair(gridX, std::pair(gridY, layer)));
					delete map.at(gridX).at(gridY).at(layer);
					map.at(gridX).at(gridY).at(layer) = nullptr;
					break;
				case tileTypes::enemy:
					enemies.erase(std::pair(gridX, std::pair(gridY, layer)));
					delete map.at(gridX).at(gridY).at(layer);
					map.at(gridX).at(gridY).at(layer) = nullptr;
					break;
				case tileTypes::npc:
					npcs.erase(std::pair(gridX, std::pair(gridY, layer)));
					delete map.at(gridX).at(gridY).at(layer);
					map.at(gridX).at(gridY).at(layer) = nullptr;
					break;
				default:
					break;
			}
		}
		else
		{
			std::cout << "No tile to remove:" << gridX << "," << gridY << ":" << layer << std::endl;
		}
	}
	else
	{
		std::cout << "Trying to remove a tile that is out of bounds!" << std::endl;
	}
}

void Tilemap::SaveCurrentMap(const std::string& path)
{
	std::cout << "Saving current map to: " << path << std::endl;
	std::ofstream out(path);
	if (!out)
	{
		std::cout << "TILEMAP::COULD NOT 	 CURRENT MAP" << std::endl;
	}

	out << mapType << '\n'
		<< spawnPoint.x << '\n'
		<< spawnPoint.y << '\n'
		<< mapSize.x << '\n'
		<< mapSize.y << '\n'
		<< layerCount << '\n';

	for (unsigned int layer = 0; layer < layerCount; layer++)
	{
		for (unsigned int coll = 0; coll < mapSize.y; coll++)
		{
			for (unsigned int row = 0; row < mapSize.x; row++)
			{
				if (map.at(row).at(coll).at(layer) != nullptr) //if there isnt any tile, then put - as empty
				{

					out << map.at(row).at(coll).at(layer)->GetType() << " ";
					switch (map.at(row).at(coll).at(layer)->GetType())
					{
						case tileTypes::tile:
							out << map.at(row).at(coll).at(layer)->GetRect().left << " "
								<< map.at(row).at(coll).at(layer)->GetRect().top << " "
								<< map.at(row).at(coll).at(layer)->isCollision() << " ";
							break;
						case tileTypes::object:
							out << objectTemplates.at(map.at(row).at(coll).at(layer)->GetID())->ID << " "
								<< objects.at(std::pair(row, std::pair(coll, layer)))->GetRemainingResource() << " "
								<< map.at(row).at(coll).at(layer)->isCollision() << " ";
							break;
						case tileTypes::portal:
							out << portalTemplates.at(map.at(row).at(coll).at(layer)->GetID())->id << " "
								<< map.at(row).at(coll).at(layer)->isCollision() << " ";
							break;
						case tileTypes::enemy:
							out << enemyTemplates.at(map.at(row).at(coll).at(layer)->GetID())->id << " "
								<< enemies.at(std::pair(row, std::pair(coll, layer)))->IsDead() << " "
								<< map.at(row).at(coll).at(layer)->isCollision() << " ";
							break;
						case tileTypes::npc:
							out << npcTemplates.at(map.at(row).at(coll).at(layer)->GetID())->id << " "
								<< npcs.at(std::pair(row, std::pair(coll, layer)))->IsDead() << " "
								<< map.at(row).at(coll).at(layer)->isCollision() << " ";
							break;
						default:
							break;
					}
				}
				else
				{
					out << "- "; //meaning that there is no tile
				}
			}
			out << '\n';
		}
	}
	out.close();

	std::cout << "Current map saved" << std::endl;
}

void Tilemap::LoadMap(const std::string& path)
{
	std::cout << "Loading map from: " << path << std::endl;
	std::ifstream inp(path); //otevření souboru
	if (!inp)
	{
		std::cout << "TILEMAP::COULD NOT LOAD MAP" << std::endl;
	}

	//načtení prvních informací o mapě
	inp >> mapType >> spawnPoint.x >> spawnPoint.y >> mapSize.x >> mapSize.y >> layerCount;

	//pomocné proměnné pro procházení mapy
	unsigned int rows = 0;
	unsigned int colls = 0;
	unsigned int layers = 0;

	//udá velikost mapy
	map.resize(mapSize.x, std::vector<std::vector<Tile*>>());
	for (auto& coll : map)
	{
		coll.resize(mapSize.y, std::vector<Tile*>());
		for (auto& layers : coll)
		{
			layers.resize(layerCount, nullptr);
		}
	}

	//vyčistí mapu
	for (unsigned int row = 0; row < mapSize.x; row++)
	{
		for (unsigned int coll = 0; coll < mapSize.x; coll++)
		{
			for (unsigned int layer = 0; layer < layerCount; layer++)
			{
				map.at(row).at(coll).at(layer) = nullptr;
			}
		}
	}

	//vyčištění map speciálních dlaždic
	enemies.clear();
	objects.clear();
	portals.clear();
	npcs.clear();

	sf::IntRect tempRect;
	tempRect.height = gridSize;
	tempRect.width = gridSize;
	tempRect.left = 0.f;
	tempRect.top = 0.f;

	std::string stream;
	bool collision = false;
	int objectID = 0;
	int remainingResources = 0;
	int mapID = 0;
	int enemyID = 0;
	bool enemyDead = false;

	int npcID = 0;
	int npcDead = false;

	int type;

	while (inp >> stream)
	{ //čte, dokuď může
		if (stream == "-")
		{ // pokud je prázdné políčko, tak se nic neudělá a čte se dál
			colls++;
			if (colls >= mapSize.x)
			{ //když se přečte řádek, resetují se sloupce a přidá se jeden řádek
				colls = 0;
				rows++;
				if (rows >= mapSize.y)
				{ // když se přeštou všechny řádky, přejde se na další vrstvu
					rows = 0;
					layers++;
				}
			}
		}
		else
		{ //podle typu dlaždice, se řádně vloží na dané místo daný typ dlaždice
			type = std::stoi(stream);
			switch (type)
			{
				case tileTypes::tile:
					inp >> stream; //souřadnice na ose X v textuře
					tempRect.left = std::stoi(stream);
					inp >> stream; //souřadnice na ose Y v textuře
					tempRect.top = std::stoi(stream);
					inp >> stream; //přečtení kolize
					collision = std::stoi(stream);
					//vložení na řádek, sloupec, vrstvu dlaždici
					map.at(colls).at(rows).at(layers) = new Tile(colls, rows, gridSize, tileSheet, tempRect, collision);
					break;
				case tileTypes::object:
					inp >> stream;
					objectID = std::stoi(stream);
					inp >> stream;
					remainingResources = std::stoi(stream);
					inp >> stream;
					collision = std::stoi(stream);
					objects[std::pair(colls, std::pair(rows, layers))] = new Object(objectID, colls, rows, gridSize, objectSheet, sf::IntRect(objectTemplates.at(objectID)->textureRect.x, objectTemplates.at(objectID)->textureRect.y, 32, 32), collision, objectTemplates.at(objectID)->name, objectTemplates.at(objectID)->hardness, objectTemplates.at(objectID)->resourceType, remainingResources, objectTemplates.at(objectID)->destroyedTex);
					map.at(colls).at(rows).at(layers) = objects.at(std::pair(colls, std::pair(rows, layers)));
					if (remainingResources < 1)
					{
						objects.at(std::pair(colls, std::pair(rows, layers)))->ChangeTextureWhenDestroyed();
					}
					break;
				case tileTypes::portal:
					inp >> stream;
					mapID = std::stoi(stream);
					inp >> stream;
					collision = std::stoi(stream);
					portals[std::pair(colls, std::pair(rows, layers))] = new Portal(colls, rows, gridSize, portalSheet, sf::IntRect(portalTemplates.at(mapID)->texCoords.x, portalTemplates.at(mapID)->texCoords.y, gridSize, gridSize), collision, mapID, 1);
					map.at(colls).at(rows).at(layers) = portals.at(std::pair(colls, std::pair(rows, layers)));
					break;
				case tileTypes::enemy:
					inp >> stream;
					enemyID = std::stoi(stream);
					inp >> stream;
					enemyDead = std::stoi(stream);
					inp >> stream;
					collision = std::stoi(stream);
					enemies[std::pair(colls, std::pair(rows, layers))] = new EnemyObject(enemyID, enemyDead, colls, rows, gridSize, enemySheet, sf::IntRect(enemyTemplates.at(enemyID)->startCoords.x, enemyTemplates.at(enemyID)->startCoords.y, gridSize, gridSize), collision, enemyTemplates.at(enemyID)->frames);
					map.at(colls).at(rows).at(layers) = enemies.at(std::pair(colls, std::pair(rows, layers)));
					break;
				case tileTypes::npc:
					inp >> stream; // identifikátor, podle kterého se načte NPC z npcTemplates
					npcID = std::stoi(stream);
					inp >> stream; //jestli je NPC mrtvé
					npcDead = std::stoi(stream);
					inp >> stream; //načtení kolize
					collision = std::stoi(stream);
					//vložení do mapy npcs na dané souřadnice mapy NPC
					npcs[std::pair(colls, std::pair(rows, layers))] = new NPC(
						npcID, npcDead, npcTemplates.at(npcID)->hasCraftingInterface, colls, rows, gridSize, npcSheet, sf::IntRect(npcTemplates.at(npcID)->startCoords.x, npcTemplates.at(npcID)->startCoords.y, gridSize, gridSize), collision, npcTemplates.at(npcID)->frames);
					//vložení NPC na dané souřadnice do mapy
					map.at(colls).at(rows).at(layers) = npcs.at(std::pair(colls, std::pair(rows, layers)));
					break;
				default:
					break;
			}
			colls++;
			if (colls >= mapSize.x)
			{ //resetování sloupců
				colls = 0;
				rows++;
				if (rows >= mapSize.y)
				{ //resetvání řádků
					rows = 0;
					layers++;
				}
			}
		}
	}

	//zavření textového souboru
	inp.close();

	std::cout << "Map loaded successfully" << std::endl;
}

void Tilemap::CheckCollision(Player& player, float dt)
{
	sf::Vector2f playerPos = { player.GetPosition().x + (player.GetSize().x / 2.f), player.GetPosition().y + (player.GetSize().y / 2.f) };
	CheckFromToBounds(playerPos.x, playerPos.y, sf::Vector2f(96.f, 96.f));

	for (long int x = fromX; x < toX; ++x)
	{
		for (long int y = fromY; y < toY; ++y)
		{
			for (unsigned int layer = 0; layer < layerCount; ++layer)
			{
				if (map.at(x).at(y).at(layer) != nullptr && map.at(x).at(y).at(layer)->isCollision() == true)
				{
					player.CheckCollision(map.at(x).at(y).at(layer)->GetGlobalBounds(), dt);
				}
			}
		}
	}

	if (player.GetGlobalBounds().top < 0)
	{ //top map border
		player.SetPosition(player.GetPosition().x, 0.f);
	}
	if (player.GetGlobalBounds().left < 0)
	{ //bottom map border
		player.SetPosition(0.f + (player.GetSize().x / 2.f), player.GetPosition().y);
	}
	if (player.GetGlobalBounds().top > (mapSize.y * gridSize) - gridSize)
	{ //top map border
		player.SetPosition(player.GetPosition().x, (mapSize.y * gridSize) - player.GetSize().y);
	}
	if (player.GetGlobalBounds().left > (mapSize.x * gridSize) - gridSize)
	{ //bottom map border
		player.SetPosition(mapSize.x * gridSize - (player.GetSize().x / 2.f), player.GetPosition().y);
	}
}

int Tilemap::UpdateMap(float dt, Player& player)
{
	if (player.IsMining())
	{
		for (auto& a : objects)
		{
			if (a.second->ContainsPoint(player.GetMiningPoint()))
			{
				if (a.second->GetRemainingResource() > 0)
				{
					if (player.GetAttributeComponent().GetEquipedItemsAttributes().atrTypes.at(ATRTYPES::miningPower) >= a.second->GetHardness())
					{
						player.GiveItem(*itemTemplates->at(a.second->GetResourceType()));
						NotifyObservers(SubjectActions::GATHER_ITEM, itemTemplates->at(a.second->GetResourceType())->GetID());
						a.second->RemoveRemainingResources(1);
						if (a.second->GetRemainingResource() < 1)
						{
							a.second->ChangeTextureWhenDestroyed();
						}
					}
					else
					{
						NotifyObservers(SubjectActions::ORE_MINED, itemTemplates->at(a.second->GetResourceType())->GetID());
					}
				}
				else
				{
					NotifyObservers(SubjectActions::ORE_MINED, itemTemplates->at(a.second->GetResourceType())->GetID());
				}
			}
		}
		for (auto& a : portals)
		{
			if (a.second->ContainsPoint(player.GetMiningPoint()))
			{
				if (a.second->GetAccess() == true)
				{
					LoadMap(resourceman->SaveDataAt(save, portalTemplates.at(a.second->GetID())->mapPath));
					player.SetPosition(GetSpawnPointCoordinates().x, GetSpawnPointCoordinates().y);
					return 0;
				}
				else
				{
					std::cout << "Portal locked" << std::endl;
				}
			}
		}
	}
	for (auto& a : enemies)
	{
		a.second->Update(dt);
		if (a.second->IsInSenseArea(player.GetPosition()) && a.second->IsSensing())
		{
			enemySensed = a.second;
			enemySensed->SetSense(false);
			return 1;
		}
	}

	bool someoneInteractible = false;
	for (auto& a : npcs)
	{
		a.second->Update(dt);
		if (a.second->IsInSenseArea(player.GetPosition()) && a.second->IsSensing())
		{
			someoneInteractible = true;
			if (player.IsInteracting() && player.CanInteract())
			{
				player.SetDialogue(*a.second);
				player.OpenCrafting(a.second->GetID(), a.second->HasCraftingInterface());

				NotifyObservers(SubjectActions::TALK_TO_NPC, a.second->GetID());
			}
		}
	}

	if (someoneInteractible == true)
	{
		someoneInteractible = false;
		player.SetInteract(true);
	}
	else
	{
		player.SetInteract(false);
	}

	if (enemySensed)
	{
		if (!enemySensed->IsInSenseArea(player.GetPosition()))
		{
			enemySensed->SetSense(true);
		}
	}
	return 0;
}

void Tilemap::Display(sf::RenderTarget& trg, sf::Vector2f pos, sf::Vector2f sizeToRender)
{
	CheckFromToBounds(pos.x, pos.y, sizeToRender);

	for (long int x = fromX; x < toX; ++x)
	{
		for (long int y = fromY; y < toY; ++y)
		{
			for (unsigned int layer = 0; layer < layerCount; ++layer)
			{
				if (map.at(x).at(y).at(layer) != nullptr)
				{
					map.at(x).at(y).at(layer)->Display(trg);
				}
			}
		}
	}
}

void Tilemap::Display(sf::RenderTarget& trg, sf::Vector2f pos, unsigned int layer, sf::Vector2f sizeToRender)
{
	CheckFromToBounds(pos.x, pos.y, sizeToRender);

	if (layer > layerCount)
	{
		std::cout << "TILEMAP::ERROR WHEN PASSING LAYER" << std::endl;
	}
	else
	{

		for (long int x = fromX; x < toX; ++x)
		{
			for (long int y = fromY; y < toY; ++y)
			{
				if (map.at(x).at(y).at(layer) != nullptr)
				{
					map.at(x).at(y).at(layer)->Display(trg);
				}
			}
		}
	}
}

void Tilemap::DisplayDebug(sf::RenderTarget& trg, sf::Vector2f pos)
{
	CheckFromToBounds(pos.x, pos.y, static_cast<sf::Vector2f>(trg.getSize()));

	for (long int x = fromX; x < toX; ++x)
	{
		for (long int y = fromY; y < toY; ++y)
		{
			for (unsigned int layer = 0; layer < layerCount; ++layer)
			{
				if (map.at(x).at(y).at(layer) != nullptr)
				{
					map.at(x).at(y).at(layer)->Display(trg);
					if (map.at(x).at(y).at(layer)->isCollision() == true)
					{
						collisionIndicator.setPosition(map.at(x).at(y).at(layer)->GetPosition());
						trg.draw(collisionIndicator);
					}
				}
			}
		}
	}
}

void Tilemap::DisplayDebug(sf::RenderTarget& trg, unsigned int layerToDisplay, sf::Vector2f pos)
{

	CheckFromToBounds(pos.x, pos.y, static_cast<sf::Vector2f>(trg.getSize()));

	if (layerToDisplay < layerCount)
	{ //if layer count valid, display that one layer only

		for (long int x = fromX; x < toX; ++x)
		{
			for (long int y = fromY; y < toY; ++y)
			{
				if (map.at(x).at(y).at(layerToDisplay) != nullptr)
				{
					map.at(x).at(y).at(layerToDisplay)->Display(trg);
					if (map.at(x).at(y).at(layerToDisplay)->isCollision() == true)
					{
						collisionIndicator.setPosition(map.at(x).at(y).at(layerToDisplay)->GetPosition());
						trg.draw(collisionIndicator);
					}
				}
			}
		}
	}
	else
	{ //if not valid layer, displays all layers

		for (long int x = fromX; x < toX; ++x)
		{

			for (long int y = fromY; y < toY; ++y)
			{
				for (unsigned int layer = 0; layer < layerCount; ++layer)
				{
					if (map.at(x).at(y).at(layer) != nullptr)
					{
						map.at(x).at(y).at(layer)->Display(trg);
						if (map.at(x).at(y).at(layer)->isCollision() == true)
						{
							collisionIndicator.setPosition(map.at(x).at(y).at(layer)->GetPosition());
							trg.draw(collisionIndicator);
						}
					}
				}
			}
		}
	}
}

//private functions

void Tilemap::CheckFromToBounds(float x, float y, sf::Vector2f size)
{
	fromX = x - (size.x / 2.f);
	toX = x + (size.x / 2.f);
	fromY = (y - (size.y / 2.f));
	toY = (y + (size.y / 2.f));

	fromX = (fromX / gridSize);
	toX = (toX / gridSize) + 1;
	fromY = (fromY / gridSize);
	toY = (toY / gridSize) + 1;

	if (fromX < 0)
	{
		fromX = 0;
	}
	if (fromY < 0)
	{
		fromY = 0;
	}
	if (static_cast<unsigned>(toX) > mapSize.x)
	{
		toX = mapSize.x;
	}
	if (static_cast<unsigned>(toY) > mapSize.y)
	{
		toY = mapSize.y;
	}
}

void Tilemap::LoadObjectSheet(const std::string& path)
{
	if (!objectSheet.loadFromFile(path))
	{
		std::cout << "TILEMAP::ERROR COULD NOT LOAD OBJECT SHEET" << std::endl;
	}
}

void Tilemap::LoadPortalSheet(const std::string& path)
{
	if (!portalSheet.loadFromFile(path))
	{
		std::cout << "TILEMAP::ERROR COULD NOT LOAD PORTAL SHEET" << std::endl;
	}
}

void Tilemap::LoadEnemySheet(const std::string& path)
{
	if (!enemySheet.loadFromFile(path))
	{
		std::cout << "TILEMAP::ERROR COULD NOT LOAD ENEMY SHEET" << std::endl;
	}
}

void Tilemap::LoadNPCSheet(const std::string& path)
{
	if (!npcSheet.loadFromFile(path))
	{
		std::cout << "TILEMAP::ERROR COULD NOT LOAD NPC SHEET" << std::endl;
	}
}

void Tilemap::LoadObjectData(const std::string& path)
{
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD OBJECT DATA" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("objects")->first_node("object");

	const char* temp = nullptr;

	sf::Vector2f rectPos;
	std::string name = "";
	int hardness = 0;
	int resourceID = 0;
	unsigned int resourceRemaining = 0;
	sf::Vector2f destroyedTex;
	int ID = 0;

	int type = 0; // 0 objects, 1 portals

	enum strings
	{
		objID,
		objName,
		objHardness,
		objResourceID,
		remainingResources,
		texCoordsX,
		texCoordsY,
		destroyedTexCoordsX,
		destroyedTexCoordsY
	};

	std::map<std::string, int> strMap; //map to use in switch statement

	strMap["ID"] = strings::objID;
	strMap["name"] = strings::objName;
	strMap["hardness"] = strings::objHardness;
	strMap["resourceID"] = strings::objResourceID;
	strMap["remainingResources"] = strings::remainingResources;
	strMap["texCoordsX"] = strings::texCoordsX;
	strMap["texCoordsY"] = strings::texCoordsY;
	strMap["destroyedTexCoordsX"] = strings::destroyedTexCoordsX;
	strMap["destroyedTexCoordsY"] = strings::destroyedTexCoordsY;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			if (std::string(parent->name()) == "object")
			{
				type = tileTypes::object;
				switch (strMap.at(child->name()))
				{
					case strings::objID:
						temp = child->value();
						ID = std::stoi(temp);
						break;
					case strings::objName:
						name = child->value();
						break;
					case strings::objHardness:
						temp = child->value();
						hardness = std::stoi(temp);
						break;
					case strings::objResourceID:
						temp = child->value();
						resourceID = std::stoi(temp);
						break;
					case strings::remainingResources:
						temp = child->value();
						resourceRemaining = std::stoi(temp);
						break;
					case strings::texCoordsX:
						temp = child->value();
						rectPos.x = std::stoul(temp);
						break;
					case strings::texCoordsY:
						temp = child->value();
						rectPos.y = std::stoi(temp);
						break;
					case strings::destroyedTexCoordsX:
						temp = child->value();
						destroyedTex.x = std::stoi(temp);
						break;
					case strings::destroyedTexCoordsY:
						temp = child->value();
						destroyedTex.y = std::stoi(temp);
						break;
					default:
						break;
				}
			}
			else if (std::string(parent->name()) == "portal")
			{
				type = tileTypes::portal;
			}
		}
		switch (type)
		{
			case tileTypes::object:
				objectTemplates[ID] = std::unique_ptr<ObjectData>(new ObjectData(ID, name, hardness, rectPos, resourceID, resourceRemaining, destroyedTex));
				hardness = 0;
				break;
			case tileTypes::portal:
				objectTemplates[ID] = std::unique_ptr<ObjectData>(new ObjectData(ID, name, hardness, rectPos, resourceID, resourceRemaining, destroyedTex));
				break;
			default:
				break;
		}
	}

	doc.clear();
	delete temp;
	/*************xml****************/
}

void Tilemap::LoadEnemyData(const std::string& path)
{
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD ENEMY DATA" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("enemies")->first_node("enemy");

	const char* temp = nullptr;

	int id = 0;
	std::string name = "";
	int level = 0;
	int experience = 0;
	int maxExperience = 0;
	int maxHealth = 0;
	int healthRegen = 0;
	int maxMana = 0;
	int manaRegen = 0;
	int maxStamina = 0;
	int staminaRegen = 0;
	int strenght = 0;
	int critDamage = 0;
	int critChance = 0;
	int dodgeChance = 0;
	int armour = 0;
	sf::Vector2f startCoords;
	int frames = 0;

	std::string animPath = "";
	std::string animName = "";
	int startx = 0;
	int starty = 0;
	int animframes = 0;
	float frametime = 0;

	int itemID = 0;
	int dropChance = 0;
	int minDrop = 0;
	int maxDrop = 0;

	std::vector<AnimationData> animList;
	std::vector<ItemDrop> itemDropList;

	enum strings
	{
		/***************/
		Eid,
		Ename,
		Elevel,
		Eexperience,
		EmaxExperience,
		EmaxHealth,
		EhealthRegen,
		EmaxMana,
		EmanaRegen,
		Estamina,
		EmaxStamina,
		EstaminaRegen,
		Estrenght,
		EcritDamage,
		EcritChance,
		EdodgeChance,
		Earmour,
		EstartCoordsX,
		EstartCoordsY,
		Eframes,
		/*******************/
		EanimPath,
		Eanim,
		EanimName,
		EanimStartX,
		EanimStartY,
		EanimFrames,
		EanimFrameTime,
		/*****************/
		EitemDrop,
		EitemID,
		Emax,
		Emin,
		EdropChance
	};

	std::map<std::string, int> strMap; //map to use in switch statement

	strMap["id"] = strings::Eid;
	strMap["name"] = strings::Ename;
	strMap["level"] = strings::Elevel;
	strMap["experience"] = strings::Eexperience;
	strMap["maxexperience"] = strings::EmaxExperience;
	strMap["maxhealth"] = strings::EmaxHealth;
	strMap["healthregen"] = strings::EhealthRegen;
	strMap["maxmana"] = strings::EmaxMana;
	strMap["manaregen"] = strings::EmanaRegen;
	strMap["stamina"] = strings::Estamina;
	strMap["maxstamina"] = strings::EmaxStamina;
	strMap["staminaregen"] = strings::EstaminaRegen;
	strMap["maxhealth"] = strings::EmaxHealth;
	strMap["strenght"] = strings::Estrenght;
	strMap["critdamage"] = strings::EcritDamage;
	strMap["critchance"] = strings::EcritChance;
	strMap["dodgechance"] = strings::EdodgeChance;
	strMap["armour"] = strings::Earmour;
	strMap["idleCoordsStartX"] = strings::EstartCoordsX;
	strMap["idleCoordsStartY"] = strings::EstartCoordsY;
	strMap["frames"] = strings::Eframes;
	strMap["animpath"] = strings::EanimPath;
	strMap["animation"] = strings::Eanim;
	strMap["animname"] = strings::EanimName;
	strMap["startx"] = strings::EanimStartX;
	strMap["starty"] = strings::EanimStartY;
	strMap["animframes"] = strings::EanimFrames;
	strMap["frametime"] = strings::EanimFrameTime;
	strMap["itemdrop"] = strings::EitemDrop;
	strMap["itemID"] = strings::EitemID;
	strMap["chance"] = strings::EdropChance;
	strMap["max"] = strings::Emax;
	strMap["min"] = strings::Emin;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			if (std::string(parent->name()) == "enemy")
			{
				switch (strMap.at(child->name()))
				{
					case strings::Eid:
						temp = child->value();
						id = std::stoi(temp);
						break;
					case strings::Ename:
						name = child->value();
						break;
					case strings::Elevel:
						temp = child->value();
						level = std::stoi(temp);
						break;
					case strings::Eexperience:
						temp = child->value();
						experience = std::stoi(temp);
						break;
					case strings::EmaxExperience:
						temp = child->value();
						maxExperience = std::stoi(temp);
						break;
					case strings::EmaxHealth:
						temp = child->value();
						maxHealth = std::stoi(temp);
						break;
					case strings::EhealthRegen:
						temp = child->value();
						healthRegen = std::stoi(temp);
						break;
					case strings::EmaxMana:
						temp = child->value();
						maxMana = std::stoi(temp);
						break;
					case strings::EmanaRegen:
						temp = child->value();
						manaRegen = std::stoi(temp);
						break;
					case strings::EmaxStamina:
						temp = child->value();
						maxStamina = std::stoi(temp);
						break;
					case strings::EstaminaRegen:
						temp = child->value();
						staminaRegen = std::stoi(temp);
						break;
					case strings::Estrenght:
						temp = child->value();
						strenght = std::stoul(temp);
						break;
					case strings::EcritDamage:
						temp = child->value();
						critDamage = std::stoi(temp);
						break;
					case strings::EcritChance:
						temp = child->value();
						critChance = std::stoi(temp);
						break;
					case strings::EdodgeChance:
						temp = child->value();
						dodgeChance = std::stoi(temp);
						break;
					case strings::Earmour:
						temp = child->value();
						armour = std::stoi(temp);
						break;
					case strings::EstartCoordsX:
						temp = child->value();
						startCoords.x = std::stoi(temp);
						break;
					case strings::EstartCoordsY:
						temp = child->value();
						startCoords.y = std::stoi(temp);
						break;
					case strings::Eframes:
						temp = child->value();
						frames = std::stoi(temp);
						break;
					case strings::EanimPath:
						animPath = child->value();
						break;
					case strings::Eanim:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::EanimName:
									animName = grandchild->value();
									break;
								case strings::EanimStartX:
									temp = grandchild->value();
									startx = std::stoi(temp);
									break;
								case strings::EanimStartY:
									temp = grandchild->value();
									starty = std::stoi(temp);
									break;
								case strings::EanimFrames:
									temp = grandchild->value();
									animframes = std::stoi(temp);
									break;
								case strings::EanimFrameTime:
									temp = grandchild->value();
									frametime = std::stof(temp);
									break;
								default:
									break;
							}
						}
						animList.push_back(AnimationData(animName, startx, starty, animframes, frametime));
						break;
					case strings::EitemDrop:
						for (rapidxml::xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
						{
							switch (strMap.at(grandchild->name()))
							{
								case strings::EitemID:
									temp = grandchild->value();
									itemID = std::stoi(temp);
									break;
								case strings::EdropChance:
									temp = grandchild->value();
									dropChance = std::stoi(temp);
									break;
								case strings::Emax:
									temp = grandchild->value();
									maxDrop = std::stoi(temp);
									break;
								case strings::Emin:
									temp = grandchild->value();
									minDrop = std::stoi(temp);
									break;
								default:
									break;
							}
						}
						itemDropList.push_back(ItemDrop(itemID, dropChance, minDrop, maxDrop));
						break;
					default:
						break;
				}
			}
		}
		enemyTemplates[id] = std::unique_ptr<EnemyData>(new EnemyData(id, name, level, experience, maxExperience, maxHealth, maxHealth, healthRegen, maxMana, maxMana, manaRegen, maxStamina, maxStamina, staminaRegen, strenght, critDamage, critChance, dodgeChance, armour, startCoords, frames, animList, itemDropList, animPath));
		itemDropList.clear();
	}

	doc.clear();
	delete temp;

	for (auto& a : enemyTemplates)
	{
		Quester::LoadEnemy(a.second->id, a.second->name);
	}
	/*************xml****************/
}

void Tilemap::LoadMapPaths(const std::string& path)
{
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD MAP PATHS" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("Portals")->first_node("portal");

	const char* temp = nullptr;

	int ID = 0;
	std::string mapPath = "";
	sf::Vector2f texCoords;

	std::map<std::string, int> strMap;

	enum strings
	{
		IDmap,
		pathMap,
		texCoordsX,
		texCoordsY
	};

	strMap["id"] = strings::IDmap;
	strMap["path"] = strings::pathMap;
	strMap["texCoordsX"] = strings::texCoordsX;
	strMap["texCoordsY"] = strings::texCoordsY;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			switch (strMap.at(child->name()))
			{
				case strings::IDmap:
					temp = child->value();
					ID = std::stoi(std::string(temp));
					break;
				case strings::pathMap:
					mapPath = std::string(child->value());
					break;
				case strings::texCoordsX:
					temp = child->value();
					texCoords.x = std::stoi(std::string(temp));
					break;
				case strings::texCoordsY:
					temp = child->value();
					texCoords.y = std::stoi(std::string(temp));
					break;
				default:
					break;
			}
		}
		portalTemplates[ID] = std::unique_ptr<PortalData>(new PortalData(ID, texCoords, mapPath));
	}
	doc.clear();
	delete temp;
	/*************xml****************/
}

void Tilemap::LoadNPCData(const std::string& path)
{
	/*************xml****************/
	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD NPC DATA" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("npcs")->first_node("npc");

	const char* temp = nullptr;

	int id = 0;
	std::string name = "";
	int craftingface = 0;
	sf::Vector2f startCoords;
	int frames = 0;

	enum strings
	{
		/***************/
		Eid,
		Ename,
		Ecraftinginterface,
		EstartCoordsX,
		EstartCoordsY,
		Eframes,
	};

	std::map<std::string, int> strMap; //map to use in switch statement

	strMap["id"] = strings::Eid;
	strMap["name"] = strings::Ename;
	strMap["craftinginterface"] = strings::Ecraftinginterface;
	strMap["idleCoordsStartX"] = strings::EstartCoordsX;
	strMap["idleCoordsStartY"] = strings::EstartCoordsY;
	strMap["frames"] = strings::Eframes;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			if (std::string(parent->name()) == "npc")
			{
				switch (strMap.at(child->name()))
				{
					case strings::Eid:
						temp = child->value();
						id = std::stoi(temp);
						break;
					case strings::Ename:
						name = child->value();
						break;
					case strings::Ecraftinginterface:
						temp = child->value();
						craftingface = std::stoi(temp);
						break;
					case strings::EstartCoordsX:
						temp = child->value();
						startCoords.x = std::stoi(temp);
						break;
					case strings::EstartCoordsY:
						temp = child->value();
						startCoords.y = std::stoi(temp);
						break;
					case strings::Eframes:
						temp = child->value();
						frames = std::stoi(temp);
						break;
					default:
						break;
				}
			}
		}
		npcTemplates[id] = std::unique_ptr<NPCData>(new NPCData(id, name, craftingface, startCoords, frames));
	}
	for (auto& a : npcTemplates)
	{
		Quester::LoadNPC(a.second->id, a.second->name);
	}
}
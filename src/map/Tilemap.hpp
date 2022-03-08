#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <iostream>
#include <map>
#include <rapidxml-1.13/rapidxml_ext.hpp>
#include <string>
#include <vector>

#include "managers/DialogueManager.hpp"
#include "managers/ResourceManager.hpp"

#include "ESC/entities/Player.hpp"

#include "containers/EnemyData.hpp"
#include "containers/ObjectData.hpp"
#include "containers/PortalData.hpp"
#include "containers/npcData.hpp"

#include "map/objects/EnemyObject.hpp"
#include "map/objects/NPC.hpp"
#include "map/objects/Object.hpp"
#include "map/objects/Portal.hpp"

#include "observers/AudioPlayer.hpp"
#include "observers/Quester.hpp"
#include "observers/Statistics.hpp"

enum mapTypes
{
	forest,
	cave
};

class ObjectData;
class PortalData;
class Player;
class Tile;
class Object;
class Portal;
class NPC;
class EnemyData;
class NPCData;
class ResourceManager;
class DialogueManager;
class Subject;
class Quester;
class Statistics;

class Tilemap : public Subject
{
private:
	ResourceManager* resourceman = ResourceManager::GetInstance(); //načte instanci manažera zdrojů
	DialogueManager* dialogueman = DialogueManager::GetInstance(); //načte instanci manažera dialogů

	int save = 1; //výchozí uložený slot

	std::vector<std::vector<std::vector<Tile*>>> map;					 //řádek, sloupec, vrstva
	std::map<std::pair<int, std::pair<int, int>>, Object*> objects;		 //unikatní mapa pro objekty
	std::map<std::pair<int, std::pair<int, int>>, Portal*> portals;		 //unikatní mapa pro portály
	std::map<std::pair<int, std::pair<int, int>>, EnemyObject*> enemies; //unikatní mapa pro nepřítele
	std::map<std::pair<int, std::pair<int, int>>, NPC*> npcs;			 //unikatní mapa pro NPC

	std::map<int, std::unique_ptr<Item>>* itemTemplates;		//ukazatel na předdefinované věci
	std::map<int, std::unique_ptr<PortalData>> portalTemplates; //předdefinované data portálů
	std::map<int, std::unique_ptr<ObjectData>> objectTemplates; //předdefinované data objektů
	std::map<int, std::unique_ptr<EnemyData>> enemyTemplates;	//předdefinované data nepřítelů
	std::map<int, std::unique_ptr<NPCData>> npcTemplates;		//předdefinované data NPC

	sf::Texture tileSheet;	 //textura pro tile set
	sf::Texture objectSheet; //textura pro objekty
	sf::Texture portalSheet; //textura pro portály
	sf::Texture enemySheet;	 //textura pro nepřítele
	sf::Texture npcSheet;	 //textura pro NPC

	sf::RectangleShape collisionIndicator; //indikátor kolize
	EnemyObject* enemySensed;			   //poslední nepřítel, se kterým hráč bojoval

	sf::Vector2u mapSize;	 //velikost mapy
	unsigned int layerCount; // počet vrstev
	int gridSize;			 //velikost jedné dlaždice
	sf::Vector2f spawnPoint; //bod, kde se hráč objeví, když se načte mapa
	int mapType;			 //typ mapy, třeba les nebo jeskyně

	//souřadnice, od kuď do kuď vykreslovat objekty
	long int fromX;
	long int toX;
	long int fromY;
	long int toY;

public:
	Tilemap(sf::Vector2u mapSize, int gridSize, unsigned int layerCount, std::map<int, std::unique_ptr<Item>>* itemTemplates = nullptr);
	~Tilemap();

	//accessors
public:
	//returns tile sheet that is using this map
	sf::Texture& GetTileSheet();

	//returns object sheet that objects are using
	sf::Texture& GetObjectSheet();

	//returns portal sheet that portals are using
	sf::Texture& GetPortalSheet();

	//returs enemy sheet
	sf::Texture& GetEnemySheet();

	//returns npc sheet
	sf::Texture& GetNPCSheet();

	//returns number of layers in map
	unsigned int GetNOLayer();

	//returns size of map in grids
	sf::Vector2i getMapGridSize();

	//returns reference to itemtemplates
	std::map<int, std::unique_ptr<Item>>& GetItemTemplates();

	//returns spawnpooint coordinates
	sf::Vector2f GetSpawnPointCoordinates();

	//returns enemy that is sensed
	EnemyObject& GetEnemySensed();

	//returns data about sensed enemy
	EnemyData GetEnemySensedData();

	int GetMapType();

	//functions
public:
	//inits map
	void InitMap(std::map<int, std::unique_ptr<Item>>* itemTemplates = nullptr);

	//loads tilesheet
	void LoadTileSheet(const std::string& tileTexture, const std::string& objectTexture, const std::string& objectData, const std::string& portalTexture, const std::string& portalPaths, const std::string& enemyTexture, const std::string& enemyData, const std::string& npcTexture, const std::string& npcData);

	//adds tile to tilemap at grid positionB
	void PlaceTile(unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, const sf::IntRect& tileRect, bool isCollision);

	//adds object to tilempa at grid position
	void PlaceObject(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision);

	//adds portal to tilemap at grid position
	void PlacePortal(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision);

	//places enemy to tilemap at grid position
	void PlaceEnemy(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision);

	//places npc to tilemap at grid position
	void PlaceNPC(int id, unsigned int gridX, unsigned int gridY, bool canReplace, unsigned int layer, bool isCollision);

	//Removes tile from map at grid position
	void RemoveTile(unsigned int gridX, unsigned int gridY, unsigned int layer);

	//saves map to .txt file, used for saving from editor
	void SaveMap(const std::string& path);

	//saves map from current map opened to a .txt file or overwrites .txt file, used for saving map in game
	void SaveCurrentMap(const std::string& path);

	//loads map from .txt file
	void LoadMap(const std::string& path /*takhle to udělej*/);

	//checks collision
	void CheckCollision(Player& player, float dt);

	//updates map
	int UpdateMap(float dt, Player& player);

	//displays map
	void Display(sf::RenderTarget& trg, sf::Vector2f pos, sf::Vector2f sizeToRender);

	//displays only one layer of map
	void Display(sf::RenderTarget& trg, sf::Vector2f pos, unsigned int layer, sf::Vector2f sizeToRender);

	//displays map in debug mode - used in editor state
	void DisplayDebug(sf::RenderTarget& trg, sf::Vector2f pos);

	//displays only one layer, if not valid, displays them all in debug mode - used in editor state
	void DisplayDebug(sf::RenderTarget& trg, unsigned int layerToDisplay, sf::Vector2f pos);

private:
	//checks for bounds fromX/fromY to toX/toY
	void CheckFromToBounds(float x, float y, sf::Vector2f size);

	//loads objects from a file
	void LoadObjectSheet(const std::string& path);

	//loads portal sheet from file
	void LoadPortalSheet(const std::string& path);

	//Loads enemy sheet from file
	void LoadEnemySheet(const std::string& path);

	//loads npc sheet from file
	void LoadNPCSheet(const std::string& path);

	//loads object data
	void LoadObjectData(const std::string& path);

	//Loads enemy data
	void LoadEnemyData(const std::string& path);

	//loads map paths
	void LoadMapPaths(const std::string& path);

	//loads npc data
	void LoadNPCData(const std::string& path);
};

#endif
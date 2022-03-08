#ifndef COMBATMANAGER_HPP
#define COMBATMANAGER_HPP

#include "ESC/components/GraphicalInventoryComponent.hpp"
#include "ESC/entities/Fighter.hpp"
#include "ESC/entities/Player.hpp"
#include "ResourceManager.hpp"
#include "containers/EnemyData.hpp"
#include "containers/FightResults.hpp"
#include "items/Item.hpp"

enum FightStates
{
	PLAYERINPUT,	 //čeká na vstup hráče
	PLAYERATTACKING, //přehrává se animace hráče
	ENEMYANSWERING,	 //přehrává se animace odpověďi nepřítele
	ENEMYINPUT,		 //nepřítel rozhoduje o jeho akci
	ENEMYATTACKING,	 //přehrává se animace nepřítele
	PLAYERANSWERING, //přehrává se animace odpovědi hráče
	PLAYERWON,		 //hráč vyhrál
	ENEMYWON		 //nepřítel vyhrál
};

enum MoveTypes
{
	QUICKSTRIKE, //rychlý úder
	SLASH,		 //seknutí
	REST		 //odpočinek
};

enum AnswerTypes
{
	NOTHING, //nic
	HURT,	 //zraněň
	DODGE	 //vyhnul
};

class Player;
class Fighter;
class Item;

class CombatManager
{

public:
	FightResults& results; //struktura obsahující informace o výsledku boje

	Player& player;	 //ukazatel na hráče
	EnemyData enemy; //data nepřítele

	std::map<int, std::unique_ptr<Item>>& itemTemplates; //data o předmětech

	Fighter playerFighter; //objekt, obsahující bojovníka hráče
	Fighter enemyFighter;  //objekt, obsahující bojovníka nepřítele

	int currentState; //stav, ve kterém se boj nachází
	int moveType;	  //jakou akci bojovník zvolil
	int answerType;	  //jakou odpověď na akci zvolil

	bool enemyRegenerated; //jestli regeneroval životy
	bool endFight;		   //jestli boj skončil
	bool setRewardScreen;  //jestli byla ukázana výsledková obrazovka

	InventoryComponent inv;					//objekt obsahující funknčnost inventáře
	GraphicalInventoryComponent GUIResults; //objekt pro graficky zobrazení inventáře

	std::map<std::string, std::unique_ptr<GUI::Button>> btns;			   //všechna tlačítka
	std::map<std::string, std::unique_ptr<GUI::Displaybox>> displays;	   //všechny GUI elemnty typu DisplayBox
	std::map<std::string, std::unique_ptr<GUI::DescriptionBox>> descripts; //všechny popisové GUI elementy

	std::map<std::string, sf::Text> texts; //všechny texty

	GUI::AttributeInfo playerInfo; //informace hráčských atributů
	GUI::AttributeInfo enemyInfo;  //informace nepřátelských atributů

	ItemData tempData;	 //pomocná proměnná o datech předmětu
	sf::Texture tempTex; //pomocná textura

	sf::Font& font; //použití font

	float keyTime;
	float keyTimeMax;

	//constructors
public:
	CombatManager(Player& player, EnemyData enemy, sf::Texture& playerTexture, sf::Texture& enemyTex, std::map<int, std::unique_ptr<Item>>& items, sf::Texture& itemSheet, FightResults& results, sf::Font& font, sf::Vector2f res);
	CombatManager();
	~CombatManager();

public:
	void AfterInit(sf::Vector2f res);

	void Update(float dt, const sf::Vector2i& mouseWinPosition);

	bool GetFightResults();

	void Display(sf::RenderTarget& trg);

	void DisplayGUI(sf::RenderTarget& trg);

private:
	void InitAll(sf::Vector2f res);

	void InitFightMenu(sf::Vector2f res);

	void InitAttributes();

	void InitTexts(sf::Vector2f res);

	//updates key time
	void updateKeyTime(float dt);

	//gets key time
	bool getKeyTime();

	void UpdateGUI(sf::Vector2i mouseWinPosition, float dt);

	void PlayerInput(float dt);
	void PlayerAttacking(float dt);
	void EnemyAnswering(float dt);
	void EnemyInput(float dt);
	void EnemyAttacking(float dt);
	void PlayerAnswering(float dt);
	void PlayerWon(float dt);
	void EnemyWon(float dt);

	void AnswerQuickStrike(bool isPlayer);
	void AnswerSlash(bool isPlayer);
	void AnswerRest();

	void RegenerateHealth(bool isPlayer);
	void RegenerateStamina(bool isPlayer);

	void SetResultItems();
};

#endif
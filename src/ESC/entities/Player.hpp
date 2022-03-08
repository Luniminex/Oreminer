#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ESC/entities/Entity.hpp"
#include "containers/EnemyData.hpp"
#include "map/objects/NPC.hpp"
#include "observers/AudioPlayer.hpp"
#include "observers/Quester.hpp"
#include "observers/Statistics.hpp"

class Item;
class BodyComponent;
class MovementComponent;
class AnimationComponent;
class HitboxComponent;
class MiningComponent;
class InventoryComponent;
class GraphicalComponent;
class AttributeComponent;
class DialogueComponent;
class QuestComponent;
class Quester;
class AudioPlayer;
class Subject;
class DialogueManager;
class CraftingComponent;
class LevelComponent;

//player entity that is controlled by the player
class Player : public Entity, public Subject
{
private:
	//komponenty
	BodyComponent bodyComp;					  //komponent, co má v sobě sprite a texturu
	MovementComponent movementComp;			  //komponent zajitující pohyb
	AnimationComponent animationComp;		  //komponent pro pohyb
	HitboxComponent hitboxComp;				  //komponentt pro kolizi
	MiningComponent miningComp;				  //komponent pro kopání
	InventoryComponent inventoryComp;		  //komponent inventáře
	GraphicalInventoryComponent graphInvComp; //komponent grafického inventáře
	AttributeComponent attrComponent;		  //komponent atributů
	DialogueComponent dialogueComp;			  //komponent pro dialogy
	QuestComponent questComp;				  //komponent pro mise
	CraftingComponent craftComp;			  //komponent pro vyrábění předmětů
	LevelComponent levelComp;				  //komponent pro zvyšování úrovně

	std::map<std::string, int>* keybinds; //ovládací klávesy hráče

	std::vector<AnimationData> animationList; //list animací pro stav boje

	sf::Vector2f lastMapPos; //ukládá poslední pozici před vstupem do boje na mapě

	bool canInteract;	   //jestli hráč může použít interakci
	bool interacting;	   //říká, jestli hráč zrovna používá interakci
	sf::Text InteractText; //text, který se zobrazí, pokud hráč může použít interakci

	//accessors
public:
	std::string fightsceneTexPath = "content/Player/fightscene/player-animation-sheet.png"; //temporary solution
	//returns float rect of hitbox
	sf::FloatRect GetGlobalBounds();

	//returns current position of player - not center
	sf::Vector2f GetPosition();

	//returns size of player
	sf::Vector2f GetSize();

	//returns layer position
	unsigned int GetZ();

	//returns true if is attacking
	bool IsMining();

	/*
	*	returns point where player mined
	*	note that it will mine regardless if player mined through input
	*	use IsMining fuction to check if player wanted to mine
	*/
	sf::Vector2f GetMiningPoint();

	InventoryComponent& GetInventoryComponent();

	AttributeComponent& GetAttributeComponent();

	BodyComponent& GetBodyComponent();

	QuestComponent& GetQuestComponent();

	//returns last map position
	sf::Vector2f GetLastMapPos();

	std::vector<AnimationData>& GetAnimationList();

	//returns if can interact
	bool CanInteract();

	//returs true if player is interacting now
	bool IsInteracting();

	//modifiers
public:
	//sets player position to coords
	void SetPosition(float x, float y);

	//gives player an item
	void GiveItem(Item& item);

	//gives player an item
	void GiveItem(Item& item, int amount);

	//saves items from inventory
	void SaveItems(const std::string& path);

	//saves player attributes
	void SaveAttributes(const std::string& path);

	//loads items
	void LoadItems(const std::string& path, std::map<int, std::unique_ptr<Item>>& itemTemplates);

	//loads players attributes
	void LoadAttributes(const std::string& path);

	//sets last map position
	void SetLastMapPosition(sf::Vector2f pos);

	//sets interact
	void SetInteract(bool canInteract);

	//loads and sets dialogue
	void SetDialogue(NPC& npc);

	//sets current quest
	void SetQuest(Quest& quest, std::vector<std::string>& goalNames, std::vector<std::string>& rewardName);

	//tells playyer that goals are completed
	void CompleteGoals();

	void OpenCrafting(int IcraftID, bool hasCrafting);

	void TryToOpenCrafting();

	void GiveExp(int experience);

	//initializer functions
private:
	//inits all components
	void InitComponents(sf::Texture* tex);

	//inits all animations
	void InitAnimations();
	//functions
public:
	Player(sf::Texture* tex, std::map<std::string, int>* keybinds, sf::Font& font, sf::Texture& itemTexture, std::map<int, std::vector<Recipe>>& recipes, std::map<int, std::unique_ptr<Item>>& itemTemplates, sf::Vector2f res);

	//reinitilaizes GUI elements of player
	void AfterInitGUI(sf::Vector2f res);

	//checks collision
	void CheckCollision(sf::FloatRect rect, float dt);

	//updates every frame
	void Update(const float& dt, sf::Vector2i& mousePosWindow);

	//renders player's GUI to screen
	void DisplayGUI(sf::RenderTarget& target);

	//display player to target window
	void Display(sf::RenderTarget& target);

	~Player();

private:
	//gets input
	void Input();

	//inits interact text
	void InitInteractText(sf::Font& font, sf::Vector2f res);

	//plays all animations
	void PlayAnimations(const float& dt);

	void UpdateGraphComponent(sf::Vector2i mousePosWindow, float dt);
};

#endif

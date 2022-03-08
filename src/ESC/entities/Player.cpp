#include "Player.hpp"
//constructor / desctructor

Player::Player(sf::Texture* tex, std::map<std::string, int>* keybinds, sf::Font& font, sf::Texture& itemTexture, std::map<int, std::vector<Recipe>>& recipes, std::map<int, std::unique_ptr<Item>>& itemTemplates, sf::Vector2f res) :
	Entity(),
	bodyComp(*tex, sf::Vector2f(1.f, 1.f)),
	movementComp(160.f, 11.f, 6.f, 225.f, 420.f, 0),
	animationComp(bodyComp.GetSprite(), *tex),
	hitboxComp(bodyComp.GetSprite(), sf::Vector2f(32.f, 32.f)),
	miningComp(sf::Vector2f(20.f, 0.f)),
	inventoryComp(),
	graphInvComp(13.5f, 20.f, 10, font, itemTexture, sf::Vector2f(13.666f, 3.666f), inventoryComp, res),
	attrComponent(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font, res),
	dialogueComp(font, res),
	questComp(res, font),
	craftComp(inventoryComp, itemTexture, recipes, itemTemplates, font, res),
	levelComp(attrComponent, font, res),
	InteractText()
{
	this->keybinds = keybinds;
	this->canInteract = false;
	this->interacting = false;

	InitAnimations();
	InitInteractText(font, res);

	dialogueComp.Show(false);

	RegisterObserver(std::make_shared<Quester>());
	RegisterObserver(std::make_shared<Statistics>());
	RegisterObserver(std::make_shared<AudioPlayer>());
}

void Player::AfterInitGUI(sf::Vector2f res)
{
	questComp.AfterInit(res);
	graphInvComp.AfterInit(res);
	attrComponent.AfterInit(res);
	craftComp.AfterInit(res);
	levelComp.AfterInit(res);
}

void Player::CheckCollision(sf::FloatRect rect, float dt)
{

	if (hitboxComp.Collided(rect, movementComp.GetVelocity(), dt) == true) //if there was any collision
	{

		switch (hitboxComp.CollidedRight(rect)) //checks on what side it is and does stuff
		{
			case CollisionSide::top:
				movementComp.ResetVelocity();
				movementComp.posy = rect.top + rect.height;
				bodyComp.GetSprite().setPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				hitboxComp.SetPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				break;
			case CollisionSide::down:
				movementComp.ResetVelocity();
				movementComp.posy = rect.top - hitboxComp.GetSize().y;
				bodyComp.GetSprite().setPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				hitboxComp.SetPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				break;
			case CollisionSide::left:
				movementComp.ResetVelocity();
				movementComp.posx = rect.left + rect.width + (hitboxComp.GetSize().x / 2.f);
				bodyComp.GetSprite().setPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				hitboxComp.SetPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				break;
			case CollisionSide::right:
				movementComp.ResetVelocity();
				movementComp.posx = rect.left - (hitboxComp.GetSize().x / 2.f);
				bodyComp.GetSprite().setPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				hitboxComp.SetPosition(sf::Vector2f(movementComp.posx, movementComp.posy));
				break;
			default:
				break;
		}
	}
}

//accessors

sf::Vector2f Player::GetPosition()
{
	return sf::Vector2f(movementComp.posx, movementComp.posy);
}

sf::Vector2f Player::GetSize()
{
	return hitboxComp.GetSize();
}

unsigned int Player::GetZ()
{
	return movementComp.GetZ();
}

bool Player::IsMining()
{
	return miningComp.mined;
}

sf::Vector2f Player::GetMiningPoint()
{
	if (animationComp.GetIsFlipped())
	{
		return miningComp.MineLeft(hitboxComp.GetGlobalBounds());
	}
	else
	{
		return miningComp.MineRight(hitboxComp.GetGlobalBounds());
	}
}

InventoryComponent& Player::GetInventoryComponent()
{
	return inventoryComp;
}

AttributeComponent& Player::GetAttributeComponent()
{
	std::cout << "Returning attrcomp" << std::endl;
	return attrComponent;
}

BodyComponent& Player::GetBodyComponent()
{
	return bodyComp;
}

QuestComponent& Player::GetQuestComponent()
{
	return questComp;
}

sf::Vector2f Player::GetLastMapPos()
{
	return lastMapPos;
}

std::vector<AnimationData>& Player::GetAnimationList()
{
	return animationList;
}

bool Player::CanInteract()
{
	return canInteract;
}

bool Player::IsInteracting()
{
	return interacting;
}

/*********modifiers************/

void Player::SetPosition(float x, float y)
{
	movementComp.posx = x;
	movementComp.posy = y;
}

void Player::GiveItem(Item& item)
{
	inventoryComp.GiveItem(item);
	for (unsigned int i = 0; i < inventoryComp.GetSizeOfVector(); ++i)
	{
		std::cout << "Player has " << inventoryComp.GetItemAt(i).GetCount() << " " << inventoryComp.GetItemAt(i).GetName() << std::endl;
	}
}

void Player::GiveItem(Item& item, int amount)
{
	inventoryComp.GiveItem(item, amount);
}

void Player::SaveItems(const std::string& path)
{
	inventoryComp.SaveInventory(path);
}

void Player::SaveAttributes(const std::string& path)
{
	attrComponent.SaveAttributes(path);
}

void Player::LoadItems(const std::string& path, std::map<int, std::unique_ptr<Item>>& itemTemplates)
{
	inventoryComp.LoadInventory(path, itemTemplates);
	for (auto& a : inventoryComp.GetEquipeditems())
	{
		attrComponent.IncreaseEquipedAttributes(a.second->GetItemData().attrs);
		attrComponent.UpdateAttributeInfo();
	}
}

void Player::LoadAttributes(const std::string& path)
{
	attrComponent.LoadAttributes(path);
}

void Player::SetLastMapPosition(sf::Vector2f pos)
{
	lastMapPos = pos;
}

void Player::SetInteract(bool canInteract)
{
	this->canInteract = canInteract;
}

void Player::SetDialogue(NPC& npc)
{
	DialogueManager* dialogueman = DialogueManager::GetInstance();
	if (Statistics::GetStatisticsContainer().npcsSeen.at(npc.GetID()) == false)
	{
		std::cout << "WELCOME DIALOGUE" << std::endl;
		dialogueComp.SetDialogue(npc.GetID(), "welcome", dialogueman->GetRandomVersionDialogue(npc.GetID(), "welcome"));
		return;
	}
	else if (questComp.IsActiveQuest() == true && questComp.AreGoalsCompleted() == false)
	{
		std::cout << "RANDOM DIALOGUE" << std::endl;
		dialogueComp.SetDialogue(npc.GetID(), "random", dialogueman->GetRandomVersionDialogue(npc.GetID(), "random"));
		return;
	}
	else if (questComp.AreGoalsCompleted() == true && questComp.IsReadyToQuest() == false) // pokud active quest je hotovy a neni pripraven na dalsi quest
	{
		for (auto& b : dialogueman->GetNPCDialogueVersions(npc.GetID(), "giveQuest")) //projed u daneho npc vsechyn questy typu questComplete
		{
			if (questComp.GetActiveQuest().id == b)
			{ //pokud npc ma quest id jako quest ktery ma activni a splneny
				dialogueComp.SetDialogue(npc.GetID(), "completeQuest", questComp.GetActiveQuest().id);
				NotifyObservers(SubjectActions::COMPLETE_QUEST, questComp.GetActiveQuest().id);
				return;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < Statistics::GetStatisticsContainer().questCompleted.size(); i++)
		{
			if (Statistics::GetStatisticsContainer().questCompleted.at(i) == false) //quest neni splneny, checkni jestli je unlocknuty
			{
				for (auto& a : Quester::quests) //projed questami, najdi ID, ktere se shoduje
				{
					if (static_cast<unsigned>(a.id) == i) //quest se stejnym id
					{
						if (a.state == QuestStates::unlocked) // quest je unlocknuty
						{
							for (auto& b : dialogueman->GetNPCDialogueVersions(npc.GetID(), "giveQuest")) // loopnout pres vsechny verze ktere to npc ma (u questy verze koresponduje s id questu)
							{
								if (static_cast<unsigned>(b) == i)
								{
									std::cout << "GIVE QUEST DIALOGUE" << std::endl;
									dialogueComp.SetDialogue(npc.GetID(), "giveQuest", a.id);
									NotifyObservers(SubjectActions::GIVE_QUEST, a.id);
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	std::cout << "RANDOM DIALOGUE" << std::endl;
	dialogueComp.SetDialogue(npc.GetID(), "random", dialogueman->GetRandomVersionDialogue(npc.GetID(), "random"));
	NotifyObservers(SubjectActions::TALK_TO_NPC, npc.GetID());
}

void Player::SetQuest(Quest& quest, std::vector<std::string>& goalNames, std::vector<std::string>& rewardName)
{
	questComp.SetQuest(quest, goalNames, rewardName);
}

void Player::CompleteGoals()
{
	questComp.Complete();
}

void Player::OpenCrafting(int IcraftID, bool hasCrafting)
{
	if (hasCrafting)
	{
		craftComp.SetRecipes(IcraftID);
	}
	craftComp.SetHasRecipes(hasCrafting);
}

void Player::TryToOpenCrafting()
{
	if (dialogueComp.IsFinished() == true)
	{
		craftComp.Show(true);
		std::cout << "Opened crafting menu" << std::endl;
	}
}

void Player::GiveExp(int experience)
{
	if (attrComponent.AddExperience(experience) == 1)
	{
		levelComp.LevelUP();
		NotifyObservers(SubjectActions::LEVEL_UP);
	}
	attrComponent.UpdateAttributeInfo();
}

//initializer funcs

void Player::InitAnimations()
{
	animationComp.CreateAnimation("PLAYER_IDLE", bodyComp.GetSprite(), bodyComp.GetTexture(), 32, 32, 0, 0, 7, 13.f);
	animationComp.CreateAnimation("PLAYER_WALK", bodyComp.GetSprite(), bodyComp.GetTexture(), 32, 32, 0, 1, 7, 8.f);
	animationComp.CreateAnimation("PLAYER_MINE", bodyComp.GetSprite(), bodyComp.GetTexture(), 32, 32, 0, 2, 5, 16.f);
	bodyComp.GetSprite().setOrigin(16.f, 0);

	animationList.push_back(AnimationData("IDLE", 0, 0, 3, 11));
	animationList.push_back(AnimationData("ATTACK", 0, 1, 11, 8));
	animationList.push_back(AnimationData("HURT", 0, 2, 4, 8));
	animationList.push_back(AnimationData("DEAD", 0, 3, 8, 11));
	animationList.push_back(AnimationData("DODGE", 0, 4, 8, 11));
	animationList.push_back(AnimationData("BLANK", 0, 0, 0, 0));
}

void Player::PlayAnimations(const float& dt)
{
	if (miningComp.mining == false)
	{
		miningComp.mined = false;
	}
	if (animationComp.GetEndedPriorityAnim() == true) //when animation ends, end mining
	{
		miningComp.mining = false;
		miningComp.mined = true;
	}

	//attackign state
	if (miningComp.mining == true)
	{
		animationComp.Play("PLAYER_MINE", true, dt);
		miningComp.mined = false;
	}

	// if idle play idle, else walk
	if (movementComp.GetState(MVMNT_STATES::IDLE))
	{
		animationComp.Play("PLAYER_IDLE", dt, false);
	}

	//mvnt state--------
	if (movementComp.GetState(MVMNT_STATES::LEFT))
	{
		animationComp.Flip(false); //flip sprite if moving left
		animationComp.Play("PLAYER_WALK", dt, false, std::abs((movementComp.GetVelocity().x / movementComp.GetMaxVelocity()) * 100));
	}
	else if (movementComp.GetState(MVMNT_STATES::RIGHT))
	{
		animationComp.Flip(true); //reset flip
		animationComp.Play("PLAYER_WALK", dt, false, std::abs((movementComp.GetVelocity().x / movementComp.GetMaxVelocity()) * 100));
	}
	else if (movementComp.GetState(MVMNT_STATES::UP))
	{
		animationComp.Play("PLAYER_WALK", dt, false, std::abs((movementComp.GetVelocity().y / movementComp.GetMaxVelocity()) * 100));
	}
	else if (movementComp.GetState(MVMNT_STATES::DOWN))
	{
		animationComp.Play("PLAYER_WALK", dt, false, std::abs((movementComp.GetVelocity().y / movementComp.GetMaxVelocity()) * 100));
	}
}

//gets input from keyboard
void Player::Input()
{
	if (miningComp.mining == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("MOVE_LEFT"))))
		{
			movementComp.Move(-1.f, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("MOVE_RIGHT"))))
		{
			movementComp.Move(1.f, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("MOVE_UP"))))
		{
			movementComp.Move(0.f, -1.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("MOVE_DOWN"))))
		{
			movementComp.Move(0.f, 1.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("ATTACK"))) && GetKeyTime())
		{
			miningComp.mining = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("OPEN_INVENTORY"))) && GetKeyTime())
	{
		graphInvComp.Show();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("OPEN_ATTRIBUTES"))) && GetKeyTime())
	{
		attrComponent.Show();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("INTERACT"))) && GetKeyTime() && canInteract == true)
	{
		std::cout << "Player trying to interact" << std::endl;
		interacting = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("NEXT_PAGE"))) && GetKeyTime())
	{
		std::cout << "Player wants another page of dialogue" << std::endl;
		dialogueComp.NextDialogue();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds->at("DEBUG"))) && GetKeyTime())
	{
		int id;
		std::cin >> id;
		inventoryComp.GiveItem(*craftComp.GetTemplates().at(id));
	}
}

//public functions
void Player::Update(const float& dt, sf::Vector2i& mousePosWindow)
{
	interacting = false;

	UpdateKeyTime(dt);
	Input(); //vstup hráče

	craftComp.Update(mousePosWindow, dt); //aktualizace komponentu vyrábění

	if (dialogueComp.Update()) //aktualizace komponentu dialogů
	{						   //pokud  si hráč žádá o další stránků dialogu
		NotifyObservers(SubjectActions::NEXT_PAGE);
	}
	TryToOpenCrafting(); //pokud dialog skončil a NPC má rozhraní pro vyrábění předmětů, otevře ho

	attrComponent.Update(dt); //aktualizace komponentu atributů

	UpdateGraphComponent(mousePosWindow, dt); //aktualizace komponentu grafického inventáře

	questComp.Update(); //aktualizace komponentu misí

	levelComp.Update(mousePosWindow); //aktualizace komponentu zvýšení úrovně

	if (inventoryComp.IsUpdated() == false)
	{								   //jestli se něco stalo v inventáři
		inventoryComp.Changed();	   //něco se stalo v inventáři
		graphInvComp.ClearInventory(); //vyčištění grafického inventáře
		graphInvComp.LoadInventory();  //načtení předmětů do grafického inventáře
		inventoryComp.Update();		   //aktualizace komponentu inventáře

		attrComponent.ResetEquipedAttrs();
		for (auto& a : inventoryComp.GetEquipeditems())
		{
			attrComponent.IncreaseEquipedAttributes(a.second->GetItemData().attrs);
			attrComponent.UpdateAttributeInfo();
		}
	}

	movementComp.Update(dt);															  //aktualizace komponentu pro pohyb
	bodyComp.GetSprite().setPosition(sf::Vector2f(movementComp.posx, movementComp.posy)); //nataví pozici podle komponentu pohybu

	PlayAnimations(dt); //hraje danou animaci

	hitboxComp.Update(bodyComp.GetSprite().getPosition()); //aktualizace komponentu pro kolizi

	//nastaví pozici textu pro interakci
	InteractText.setPosition(GetPosition().x - InteractText.getGlobalBounds().width / 2.f, GetPosition().y - 32.f);
}

void Player::DisplayGUI(sf::RenderTarget& target)
{
	craftComp.Display(target);
	questComp.Display(target);
	attrComponent.Display(target);
	graphInvComp.Display(target);
	dialogueComp.Display(target);
	levelComp.Display(target);
}

void Player::Display(sf::RenderTarget& target)
{
	bodyComp.Display(target);
	if (canInteract == true)
	{
		target.draw(InteractText);
	}
	//hitboxComp.Display(target);
}

Player::~Player()
{
}

sf::FloatRect Player::GetGlobalBounds()
{
	return hitboxComp.GetGlobalBounds();
}

/*****private functions*******/

void Player::InitInteractText(sf::Font& font, sf::Vector2f res)
{
	InteractText.setCharacterSize(GUI::CaclCharSizeSmallest(res));
	InteractText.setFont(font);
	InteractText.setString(std::string("Interactible"));
}

void Player::UpdateGraphComponent(sf::Vector2i mousePosWindow, float dt)
{
	if (graphInvComp.Update(mousePosWindow, dt) == true)
	{ //graphinventory update // if true, means changes were made to items equiped/unequiped, need to refresh attribute component
		attrComponent.ResetEquipedAttrs();
		for (auto& a : inventoryComp.GetEquipeditems())
		{
			std::cout << "Got item" << std::endl;
			attrComponent.IncreaseEquipedAttributes(a.second->GetItemData().attrs);
			attrComponent.UpdateAttributeInfo();
			std::cout << "Increased attributes" << std::endl;
		}
	}
}
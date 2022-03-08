#include "managers/CombatManager.hpp"

/*************constructors / destructor *************/
CombatManager::CombatManager(Player& player, EnemyData enemy, sf::Texture& playerTexture, sf::Texture& enemyTex, std::map<int, std::unique_ptr<Item>>& items, sf::Texture& itemSheet, FightResults& results, sf::Font& font, sf::Vector2f res) :
	results(results),
	player(player),
	enemy(enemy),
	itemTemplates(items),
	playerFighter(player.GetAttributeComponent(), player.GetAnimationList(), &playerTexture, res),
	enemyFighter(enemy.GetAttrs(), enemy.animationList, &enemyTex, font, res),
	inv(),
	GUIResults(44.5f, 20.f, 5, font, itemSheet, sf::Vector2f(15.f, 6.f), inv, res),
	playerInfo(sf::Vector2f(0.f, 50.f), sf::Vector2f(15.f, 15.f), font, res),
	enemyInfo(sf::Vector2f(93.f, 50.f), sf::Vector2f(15.f, 15.f), font, res),
	font(font)
{
	InitAll(res);
}

CombatManager::~CombatManager()
{
}

void CombatManager::InitAll(sf::Vector2f res)
{
	this->keyTime = 0.f;
	this->keyTimeMax = 15.f;

	endFight = false;
	enemyRegenerated = false;
	setRewardScreen = false;
	currentState = FightStates::PLAYERINPUT;

	btns["OKBTN"] = std::make_unique<GUI::Button>(43.1f, 65, sf::Vector2f(13, 13), font, "Continue", sf::Color(125, 125, 125), sf::Color(145, 145, 145), sf::Color(115, 115, 115), res);
	btns.at("OKBTN")->SetShown(false);

	InitFightMenu(res);
	InitAttributes();
	InitTexts(res);

	playerFighter.SetPosition(sf::Vector2f(GUI::PeToPiX(26.0416f, res.x), GUI::PeToPiY(64.8148f, res.y)));
	enemyFighter.SetPosition(sf::Vector2f(GUI::PeToPiX(78.125F, res.x), GUI::PeToPiY(64.8148f, res.y)));
}

void CombatManager::InitFightMenu(sf::Vector2f res)
{

	tempTex.loadFromFile("content/items/oneitemsheet.png");
	tempData.name = "TEST";
	tempData.texPosition = { 32, 64 };
	tempData.count = 1;
	tempData.ID = 99;
	tempData.stack = 0;
	tempData.stackable = 0;

	descripts["MOVES"] = std::unique_ptr<GUI::DescriptionBox>(new GUI::DescriptionBox(sf::Vector2f(20.9f, 12.f), sf::Vector2f(20.f, 20.f), tempTex, sf::IntRect(sf::Vector2i(32, 64), sf::Vector2i(32, 32)), font, tempData, res));
	descripts.at("MOVES")->AddButton("Use");

	tempData.name = "Quick Strike";
	displays["QUICK"] = std::unique_ptr<GUI::Displaybox>(new GUI::Displaybox(8.f, 12.f, sf::Vector2f(13.f, 5.f), &tempTex, tempData, font, res));
	displays.at("QUICK")->ChangeLeftSpecial("Cost: 10 stamina");
	displays.at("QUICK")->ChangeRightSpecial("");

	tempData.name = "Slash";
	displays["SLASH"] = std::unique_ptr<GUI::Displaybox>(new GUI::Displaybox(8.f, 17.f, sf::Vector2f(13.f, 5.f), &tempTex, tempData, font, res));
	displays.at("SLASH")->ChangeLeftSpecial("Cost: 20 stamina");
	displays.at("SLASH")->ChangeRightSpecial("");

	tempData.name = "Rest";
	tempData.texPosition = { 64, 32 };
	displays["REST"] = std::unique_ptr<GUI::Displaybox>(new GUI::Displaybox(8.f, 22.f, sf::Vector2f(13.f, 5.f), &tempTex, tempData, font, res));
	displays.at("REST")->ChangeLeftSpecial("Cost: 0");
	displays.at("REST")->ChangeRightSpecial("");

	GUIResults.Show();
}

void CombatManager::InitAttributes()
{
	int i = 0;
	for (auto& a : atrNameMap)
	{
		playerInfo.AddAttribute(a + " :" + std::to_string(playerFighter.GetAttributeComponent().getAttributes().atrTypes.at(i)));
		i++;
	}
	i = 0;
	for (auto& a : atrNameMap)
	{
		enemyInfo.AddAttribute(a + " :" + std::to_string(enemyFighter.GetAttributeComponent().getAttributes().atrTypes.at(i)));
		i++;
	}
	playerInfo.Show();
	enemyInfo.Show();
}

void CombatManager::InitTexts(sf::Vector2f res)
{
	texts["PLAYERHEALTH"] = sf::Text("Health: " + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)), font, GUI::CaclCharSize(res));
	texts.at("PLAYERHEALTH").setPosition(GUI::PeToPiX(16.f, res.x), GUI::PeToPiY(47.f, res.y));
	texts["PLAYERSTAMINA"] = sf::Text("Stamina: " + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)), font, GUI::CaclCharSize(res));
	texts.at("PLAYERSTAMINA").setPosition(GUI::PeToPiX(16.f, res.x), GUI::PeToPiY(51.f, res.y));
	texts["PLAYERTEXT"] = sf::Text("", font, GUI::CaclCharSize(res));
	texts.at("PLAYERTEXT").setPosition(GUI::PeToPiX(23.f, res.x), GUI::PeToPiY(60.f, res.y));

	texts["ENEMYHEALTH"] = sf::Text("Health: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)), font, GUI::CaclCharSize(res));
	texts.at("ENEMYHEALTH").setPosition(GUI::PeToPiX(73.3333f, res.x), GUI::PeToPiY(47.f, res.y));
	texts["ENEMYSTAMINA"] = sf::Text("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)), font, GUI::CaclCharSize(res));
	texts.at("ENEMYSTAMINA").setPosition(GUI::PeToPiX(73.3333f, res.x), GUI::PeToPiY(51.f, res.y));
	texts["ENEMYTEXT"] = sf::Text("", font, GUI::CaclCharSize(res));
	texts.at("ENEMYTEXT").setPosition(GUI::PeToPiX(73.f, res.x), GUI::PeToPiY(60.f, res.y));
}

void CombatManager::AfterInit(sf::Vector2f res)
{
	texts.at("PLAYERHEALTH").setCharacterSize(GUI::CaclCharSize(res));
	texts.at("PLAYERSTAMINA").setCharacterSize(GUI::CaclCharSize(res));
	texts.at("PLAYERTEXT").setCharacterSize(GUI::CaclCharSize(res));
	texts.at("ENEMYHEALTH").setCharacterSize(GUI::CaclCharSize(res));
	texts.at("ENEMYSTAMINA").setCharacterSize(GUI::CaclCharSize(res));
	texts.at("ENEMYSTAMINA").setCharacterSize(GUI::CaclCharSize(res));

	texts.at("PLAYERHEALTH").setPosition(GUI::PeToPiX(16.f, res.x), GUI::PeToPiY(47.f, res.y));
	texts.at("PLAYERSTAMINA").setPosition(GUI::PeToPiX(16.f, res.x), GUI::PeToPiY(51.f, res.y));
	texts.at("PLAYERTEXT").setPosition(GUI::PeToPiX(23.f, res.x), GUI::PeToPiY(60.f, res.y));

	texts.at("ENEMYHEALTH").setPosition(GUI::PeToPiX(73.3333f, res.x), GUI::PeToPiY(47.f, res.y));
	texts.at("ENEMYSTAMINA").setPosition(GUI::PeToPiX(73.3333f, res.x), GUI::PeToPiY(51.f, res.y));
	texts.at("ENEMYTEXT").setPosition(GUI::PeToPiX(73.f, res.x), GUI::PeToPiY(60.f, res.y));

	playerFighter.AfterInit(res);
	enemyFighter.AfterInit(res);

	playerFighter.SetPosition(sf::Vector2f(GUI::PeToPiX(26.0416f, res.x), GUI::PeToPiY(64.8148f, res.y)));
	enemyFighter.SetPosition(sf::Vector2f(GUI::PeToPiX(78.125F, res.x), GUI::PeToPiY(64.8148f, res.y)));

	for (auto& a : btns)
	{
		a.second->AferInit(res);
	}
	for (auto& a : displays)
	{
		a.second->AfterInit(res);
	}
	for (auto& a : descripts)
	{
		a.second->AfterInit(res);
		a.second->Show(true);
	}
}

void CombatManager::UpdateGUI(sf::Vector2i mouseWinPosition, float dt)
{
	for (auto& a : btns)
	{
		a.second->Update(mouseWinPosition);
	}
	for (auto& a : displays)
	{
		a.second->Update(mouseWinPosition);
	}
	for (auto& a : descripts)
	{
		a.second->Update(mouseWinPosition, dt);
	}

	if (btns.at("OKBTN")->GetState() == btnStates::BTN_PRESSED && getKeyTime() == true)
	{
		endFight = true;
	}

	if (displays.at("QUICK")->GetState() == btnStates::BTN_PRESSED)
	{
		descripts.at("MOVES")->Show(true);
		descripts.at("MOVES")->ChangeTitle("Quick Strike");
		descripts.at("MOVES")->ChangeDescription("Fast strike enemy has a difficulty to dodge");
		descripts.at("MOVES")->ChangeLeftSpecial("Cost: 10 stamina");
	}
	if (displays.at("SLASH")->GetState() == btnStates::BTN_PRESSED)
	{
		descripts.at("MOVES")->Show(true);
		descripts.at("MOVES")->ChangeTitle("Slash");
		descripts.at("MOVES")->ChangeDescription("Sword slash which is easier to dodge");
		descripts.at("MOVES")->ChangeLeftSpecial("Cost: 20 stamina");
	}
	if (displays.at("REST")->GetState() == btnStates::BTN_PRESSED)
	{
		descripts.at("MOVES")->Show(true);
		descripts.at("MOVES")->ChangeTitle("Rest");
		descripts.at("MOVES")->ChangeDescription("Hero rests for a turn");
		descripts.at("MOVES")->ChangeLeftSpecial("Cost: 0");
	}

	if (descripts.at("MOVES")->GetButton(0).GetState() == btnStates::BTN_PRESSED && getKeyTime() == true)
	{
		descripts.at("MOVES")->Show(false);
	}
	playerInfo.Update(dt);
	enemyInfo.Update(dt);
	GUIResults.Update(mouseWinPosition, dt);
}

void CombatManager::Update(float dt, const sf::Vector2i& mouseWinPosition)
{
	AudioPlayer::CheckMusic(); //funkce pro zkontrolování muziky

	updateKeyTime(dt);
	UpdateGUI(mouseWinPosition, dt); //aktualizují se GUI prvky
	switch (currentState)
	{ //přechod do jednotlivých stavů
		case FightStates::PLAYERINPUT:
			PlayerInput(dt); //vstup hráče, výběr akce
			break;
		case FightStates::PLAYERATTACKING:
			PlayerAttacking(dt); //přehrává daná animace hráče
			break;
		case FightStates::ENEMYANSWERING:
			EnemyAnswering(dt); //přehrává se daná animace odpovědi nepřítele
			break;
		case FightStates::ENEMYINPUT:
			EnemyInput(dt); //nepřítel se rozhoduje, jakou akci použije
			break;
		case FightStates::ENEMYATTACKING:
			EnemyAttacking(dt); //přehrává se daná animace nepřítele
			break;
		case FightStates::PLAYERANSWERING:
			PlayerAnswering(dt); //přehrává se daná animace odpovědi hráče
			break;
		case FightStates::PLAYERWON:
			PlayerWon(dt); //hráč vyhrál
			break;
		case FightStates::ENEMYWON:
			EnemyWon(dt); //hráč vyhrál
			break;
		default:
			break;
	}
}

void CombatManager::updateKeyTime(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

bool CombatManager::getKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

void CombatManager::PlayerInput(float dt)
{
	if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) <= 0)
	{
		currentState = FightStates::ENEMYWON;
		return;
	}
	enemyFighter.PlayAnim("IDLE", dt);
	playerFighter.PlayAnim("IDLE", dt);
	if (descripts.at("MOVES")->GetButton(1).GetState() == btnStates::BTN_PRESSED && getKeyTime() == true)
	{
		if (descripts.at("MOVES")->GetTitle() == "Quick Strike")
		{
			if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 10)
			{
				currentState = FightStates::PLAYERATTACKING;
				moveType = MoveTypes::QUICKSTRIKE;
				playerFighter.GetAttributeComponent().DecreaseStamina(10);
				texts.at("PLAYERSTAMINA").setString("Stamina:" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
				texts.at("PLAYERTEXT").setString("Player used Quickstrike!");
				AudioPlayer::PlaySound("QUICKSTRIKE");
			}
		}

		else if (descripts.at("MOVES")->GetTitle() == "Slash")
		{
			if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 20)
			{
				currentState = FightStates::PLAYERATTACKING;
				moveType = MoveTypes::SLASH;
				playerFighter.GetAttributeComponent().DecreaseStamina(20);
				texts.at("PLAYERSTAMINA").setString("Stamina:" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
				texts.at("PLAYERTEXT").setString("Player used Slash!");
				AudioPlayer::PlaySound("SLASH");
			}
		}
		else if (descripts.at("MOVES")->GetTitle() == "Rest")
		{
			currentState = FightStates::PLAYERATTACKING;
			moveType = MoveTypes::REST;
			texts.at("PLAYERTEXT").setString("Player is resting!");
		}
	}
}

void CombatManager::PlayerAttacking(float dt)
{
	texts.at("ENEMYTEXT").setString("");
	enemyFighter.PlayAnim("IDLE", dt);
	switch (moveType)
	{
		case MoveTypes::QUICKSTRIKE:
			if (!playerFighter.IsCycleDone("ATTACK"))
			{
				playerFighter.PlayAnim("ATTACK", dt);
			}
			else
			{
				playerFighter.PlayAnim("ATTACK", dt);
				AnswerQuickStrike(false);
				currentState = FightStates::ENEMYANSWERING;
			}
			break;
		case MoveTypes::SLASH:
			if (!playerFighter.IsCycleDone("ATTACK"))
			{
				playerFighter.PlayAnim("ATTACK", dt);
			}
			else
			{
				playerFighter.PlayAnim("ATTACK", dt);
				AnswerSlash(false);
				currentState = FightStates::ENEMYANSWERING;
			}
			break;
		case MoveTypes::REST:
			if (!playerFighter.IsCycleDone("IDLE"))
			{
				playerFighter.PlayAnim("IDLE", dt);
			}
			else
			{
				playerFighter.PlayAnim("IDLE", dt);
				AnswerRest();
				currentState = FightStates::ENEMYANSWERING;
			}
			break;
		default:
			break;
	};
}

void CombatManager::EnemyAnswering(float dt)
{
	playerFighter.PlayAnim("IDLE", dt);
	switch (answerType)
	{
		case AnswerTypes::HURT:
			if (enemyFighter.IsCycleDone("HURT"))
			{
				enemyFighter.PlayAnim("HURT", dt);
				currentState = FightStates::ENEMYINPUT;
			}
			else
			{
				enemyFighter.PlayAnim("HURT", dt);
			}
			break;
		case AnswerTypes::DODGE:
			if (enemyFighter.IsCycleDone("DODGE"))
			{
				enemyFighter.PlayAnim("DODGE", dt);
				currentState = FightStates::ENEMYINPUT;
			}
			else
			{
				enemyFighter.PlayAnim("DODGE", dt);
			}
			break;
		case AnswerTypes::NOTHING:
			if (enemyFighter.IsCycleDone("IDLE"))
			{
				enemyFighter.PlayAnim("IDLE", dt);
				currentState = FightStates::ENEMYINPUT;
			}
			else
			{
				enemyFighter.PlayAnim("IDLE", dt);
			}
			break;
		default:
			break;
	}
}
void CombatManager::EnemyInput(float dt)
{

	if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) <= 0)
	{
		currentState = FightStates::PLAYERWON;
		return;
	}

	RegenerateHealth(true);
	RegenerateStamina(true);
	if (moveType == MoveTypes::REST)
	{
		RegenerateStamina(true);
	}
	enemyRegenerated = false;
	playerFighter.PlayAnim("IDLE", dt);
	enemyFighter.PlayAnim("IDLE", dt);

	bool moveChosen = false;
	if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 10)
	{
		if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) + playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::armour) <= enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::strenght))
		{ //player would die after quick strike
			if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 10 && moveChosen == false)
			{ //if he has enough stamina to use quickstrike
				moveType = MoveTypes::QUICKSTRIKE;
				moveChosen = true;
				enemyFighter.GetAttributeComponent().DecreaseStamina(10);
				texts.at("ENEMYSTAMINA").setString("Stamina:" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
			}
			else
			{ //not enough stamina to kill player, use defend to regenerate stamina
				if (moveChosen == false)
				{
					moveType = MoveTypes::REST;
					moveChosen = true;
				}
			}
		}
		if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) + playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::armour) <= (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::strenght) * 1.5))
		{ //player would die after slash
			if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 20 && moveChosen == false)
			{ //if he has enough stamina to use quickstrike
				moveType = MoveTypes::SLASH;
				moveChosen = true;
				enemyFighter.GetAttributeComponent().DecreaseStamina(20);
				texts.at("ENEMYSTAMINA").setString("Stamina:" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
			}
			else
			{ //not enough stamina to kill player, use defend to regenerate stamina
				if (moveChosen == false)
				{
					moveType = MoveTypes::REST;
					moveChosen = true;
				}
			}
		}

		if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) <= playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::strenght))
		{
			if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) <= playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth) / 3)
			{
				if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 20 && moveChosen == false)
				{
					moveType = MoveTypes::QUICKSTRIKE;
					moveChosen = true;
					enemyFighter.GetAttributeComponent().DecreaseStamina(10);
					texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
				}
				else if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) >= 10 && moveChosen == false)
				{
					moveType = MoveTypes::QUICKSTRIKE;
					moveChosen = true;
					enemyFighter.GetAttributeComponent().DecreaseStamina(10);
					texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
				}
				else if (std::rand() % 101 < 50 && moveChosen == false)
				{
					moveType = MoveTypes::REST;
					moveChosen = true;
				}
				else
				{
					if (moveChosen == false)
					{
						moveType = MoveTypes::QUICKSTRIKE;
						moveChosen = true;
						enemyFighter.GetAttributeComponent().DecreaseStamina(10);
						texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
					}
				}
			}
		}

		if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) < 20 && moveChosen == false)
		{
			moveType = MoveTypes::QUICKSTRIKE;
			moveChosen = true;
			enemyFighter.GetAttributeComponent().DecreaseStamina(10);
			texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
		}
		else if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) < 10 && moveChosen == false)
		{
			moveType = MoveTypes::SLASH;
			moveChosen = true;
			enemyFighter.GetAttributeComponent().DecreaseStamina(20);
			texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
		}

		if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) > playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth) / 2)
		{
			if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) > 10 && moveChosen == false)
			{
				moveType = MoveTypes::QUICKSTRIKE;
				moveChosen = true;
				enemyFighter.GetAttributeComponent().DecreaseStamina(10);
				texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
			}
		}
		if (playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) <= playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth) / 2)
		{
			if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) > 20 && moveChosen == false)
			{
				moveType = MoveTypes::SLASH;
				moveChosen = true;
				enemyFighter.GetAttributeComponent().DecreaseStamina(20);
				texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
			}
		}

		if (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina) < enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina) - (enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina) / 2 / 3) && moveChosen == false)
		{ //if enemy has less than two thirds of stamina, use defend to regenerate

			moveType = MoveTypes::REST;
			moveChosen = true;
		}

		if ((std::rand() % 101) > 50 && moveChosen == false)
		{
			//quick strike
			moveType = MoveTypes::QUICKSTRIKE;
			moveChosen = true;
			enemyFighter.GetAttributeComponent().DecreaseStamina(10);
			texts.at("ENEMYSTAMINA").setString("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
		}
		else
		{
			//slash
			if (moveChosen == false)
			{
				moveType = MoveTypes::SLASH;
				moveChosen = true;
				enemyFighter.GetAttributeComponent().DecreaseStamina(20);
				texts.at("ENEMYSTAMINA").setString("Stamina:" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina)));
			}
		}
	}
	else
	{
		moveType = MoveTypes::REST;
		moveChosen = true;
	}
	switch (moveType)
	{
		case MoveTypes::QUICKSTRIKE:
			texts.at("ENEMYTEXT").setString("Enemy used Quickstrike!");
			AudioPlayer::PlaySound("QUICKSTRIKE");
			break;
		case MoveTypes::SLASH:
			texts.at("ENEMYTEXT").setString("Enemy used Slash!");
			AudioPlayer::PlaySound("SLASH");
			break;
		case MoveTypes::REST:
			texts.at("ENEMYTEXT").setString("Enemy is resting!");
			break;
		default:
			break;
	};
	currentState = FightStates::ENEMYATTACKING;
}

void CombatManager::EnemyAttacking(float dt)
{
	playerFighter.PlayAnim("IDLE", dt);
	switch (moveType)
	{
		case MoveTypes::QUICKSTRIKE:
			if (enemyFighter.IsCycleDone("ATTACK"))
			{
				enemyFighter.PlayAnim("ATTACK", dt);
				AnswerQuickStrike(true);

				currentState = FightStates::PLAYERANSWERING;
			}
			else
			{
				enemyFighter.PlayAnim("ATTACK", dt);
			}
			break;
		case MoveTypes::SLASH:
			if (enemyFighter.IsCycleDone("ATTACK"))
			{
				enemyFighter.PlayAnim("ATTACK", dt);
				AnswerSlash(true);
				currentState = FightStates::PLAYERANSWERING;
			}
			else
			{
				enemyFighter.PlayAnim("ATTACK", dt);
			}
			break;
		case MoveTypes::REST:
			if (enemyFighter.IsCycleDone("IDLE"))
			{
				enemyFighter.PlayAnim("IDLE", dt);
				AnswerRest();
				currentState = FightStates::PLAYERANSWERING;
			}
			else
			{
				enemyFighter.PlayAnim("IDLE", dt);
			}
			break;
		default:
			break;
	}
}

void CombatManager::PlayerAnswering(float dt)
{
	texts.at("PLAYERTEXT").setString("");
	enemyFighter.PlayAnim("IDLE", dt);
	if (enemyRegenerated == false)
	{
		RegenerateHealth(false);
		RegenerateStamina(false);
		if (moveType == MoveTypes::REST)
		{
			RegenerateStamina(false);
		}
		enemyRegenerated = true;
	}
	switch (answerType)
	{
		case AnswerTypes::HURT:
			if (playerFighter.IsCycleDone("HURT"))
			{
				playerFighter.PlayAnim("HURT", dt);
				currentState = FightStates::PLAYERINPUT;
			}
			else
			{
				playerFighter.PlayAnim("HURT", dt);
			}
			break;
		case AnswerTypes::DODGE:
			if (playerFighter.IsCycleDone("DODGE"))
			{
				playerFighter.PlayAnim("DODGE", dt);
				currentState = FightStates::PLAYERINPUT;
			}
			else
			{
				playerFighter.PlayAnim("DODGE", dt);
			}
			break;
		case AnswerTypes::NOTHING:
			if (playerFighter.IsCycleDone("IDLE"))
			{
				playerFighter.PlayAnim("IDLE", dt);
				currentState = FightStates::PLAYERINPUT;
			}
			else
			{
				playerFighter.PlayAnim("IDLE", dt);
			}
			break;
		default:
			break;
	}
}

void CombatManager::PlayerWon(float dt)
{
	if (!setRewardScreen)
	{								 //pokud je první iterace téhle funkce, takse načtou a zobrazí určité prvky
		results.SetFightEnded(true); //inforamce o tom, že boj skončil
		results.SetWhoWon(true);	 //hodnota true indikuje, že vyhrál hráč

		//nastaví se počet zkušeností
		results.SetExpDrop(enemy.attrs.atrTypes.at(ATRTYPES::experience) + (std::rand() % enemy.attrs.atrTypes.at(ATRTYPES::experienceBar)));
		SetResultItems();			//načtou se předměty které získal jako odměnu
		GUIResults.LoadInventory(); //načtené předměty se načtou do grafického inventáře

		GUIResults.Show();				  //zobrazí se graficky inventář
		btns.at("OKBTN")->SetShown(true); //zobrazí se tlačítko, které slouží k ukončení boje a přechodu zpátky do GameState

		setRewardScreen = true; // říká, že iterace jednou proběhla
		texts.at("PLAYERTEXT").setString("Won!");
		texts.at("ENEMYTEXT").setString("Lost!");
	}
	if (enemyFighter.IsCycleDone("DEAD"))
	{ //pokud animace umírání se dohrála, tak se přestane animovat
		enemyFighter.StopAnimating();
	}
	else
	{ //přeháráv se animace umírání
		enemyFighter.PlayAnim("DEAD", dt, false);
	}
	//přehrává se animace nečinný
	playerFighter.PlayAnim("IDLE", dt);
}

void CombatManager::EnemyWon(float dt)
{
	if (!setRewardScreen)
	{
		results.SetFightEnded(true);
		results.SetWhoWon(false);
		btns.at("OKBTN")->SetShown(true);
		enemyFighter.PlayAnim("IDLE", dt);
		setRewardScreen = true;
		texts.at("PLAYERTEXT").setString("Lost!");
		texts.at("ENEMYTEXT").setString("Won!");
	}
	if (playerFighter.IsCycleDone("DEAD"))
	{
		playerFighter.StopAnimating();
	}
	else
	{
		playerFighter.PlayAnim("DEAD", dt, false);
	}
	enemyFighter.PlayAnim("IDLE", dt);
}

void CombatManager::AnswerQuickStrike(bool isPlayer)
{
	AttributeComponent& pl = playerFighter.GetAttributeComponent();
	AttributeComponent& en = enemyFighter.GetAttributeComponent();

	if (!isPlayer)
	{
		if (std::rand() % 100 > static_cast<float>(en.GetAttribute(ATRTYPES::dodgeChance)))
		{ //player hit
			float playerDamage = 0;
			float enemyArmour = 0;
			float damageDealt = 0;
			if (std::rand() % 100 < pl.GetAttribute(ATRTYPES::critChance))
			{ //player hit critical strike
				playerDamage = static_cast<float>(pl.GetAttribute(ATRTYPES::strenght)) * (static_cast<float>(pl.GetAttribute(ATRTYPES::critDamage)) / 100);
				enemyArmour = en.GetAttribute(ATRTYPES::armour);
				damageDealt = playerDamage - enemyArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			else
			{ // player hit regural strike
				playerDamage = pl.GetAttribute(ATRTYPES::strenght);
				enemyArmour = 0;
				enemyArmour = en.GetAttribute(ATRTYPES::armour);
				damageDealt = playerDamage - enemyArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			answerType = AnswerTypes::HURT;
			enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) -= damageDealt;
			texts.at("ENEMYHEALTH").setString("Health:" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)));
		}
		else
		{ //player missed
			answerType = AnswerTypes::DODGE;
			texts.at("ENEMYTEXT").setString("Dodged!");
		}
	}
	else
	{ //answering enemy to player attack
		if (std::rand() % 100 > pl.GetAttribute(ATRTYPES::dodgeChance))
		{ //enemy hit
			float enemyDamage = 0;
			float playerArmour = 0;
			float damageDealt = 0;
			if (std::rand() % 100 < en.GetAttribute(ATRTYPES::critChance))
			{ //enemy hit critical strike
				enemyDamage = static_cast<float>(en.GetAttribute(ATRTYPES::strenght)) * (static_cast<float>(en.GetAttribute(ATRTYPES::critDamage)) / 100);
				playerArmour = pl.GetAttribute(ATRTYPES::armour);
				damageDealt = enemyDamage - playerArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			else
			{ // enemy hit regural strike
				enemyDamage = en.GetAttribute(ATRTYPES::strenght);
				playerArmour = pl.GetAttribute(ATRTYPES::armour);
				damageDealt = enemyDamage - playerArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			answerType = AnswerTypes::HURT;
			playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) -= damageDealt;
			texts.at("PLAYERHEALTH").setString("Health:" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)));
		}
		else
		{ //enemy missed
			answerType = AnswerTypes::DODGE;
			texts.at("PLAYERTEXT").setString("Dodged!");
		}
	}
	switch (answerType)
	{
		case AnswerTypes::HURT:
			AudioPlayer::PlaySound("HURT");
			break;
		case AnswerTypes::DODGE:
			AudioPlayer::PlaySound("DODGE");
			break;
		case AnswerTypes::NOTHING:
			break;
		default:
			break;
	}
}

void CombatManager::AnswerSlash(bool isPlayer)
{
	AttributeComponent& pl = playerFighter.GetAttributeComponent();
	AttributeComponent& en = enemyFighter.GetAttributeComponent();
	if (!isPlayer)
	{
		if (std::rand() % 100 > (en.GetAttribute(ATRTYPES::dodgeChance) + 10))
		{ //player hit
			float playerDamage = 0;
			float enemyArmour = 0;
			float damageDealt = 0;
			if (std::rand() % 100 < pl.GetAttribute(ATRTYPES::critChance) - 5)
			{ //player hit critical strike
				playerDamage = static_cast<float>(pl.GetAttribute(ATRTYPES::strenght)) * (static_cast<float>(pl.GetAttribute(ATRTYPES::critDamage)) / 100) * 1.5;
				enemyArmour = en.GetAttribute(ATRTYPES::armour);
				damageDealt = playerDamage - enemyArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			else
			{ // player hit regural strike
				playerDamage = static_cast<float>(pl.GetAttribute(ATRTYPES::strenght)) * 1.5;
				enemyArmour = en.GetAttribute(ATRTYPES::armour);
				damageDealt = playerDamage - enemyArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			answerType = AnswerTypes::HURT;
			enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) = enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) - damageDealt;
			texts.at("ENEMYHEALTH").setString("Health:" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)));
		}
		else
		{ //player missed
			answerType = AnswerTypes::DODGE;
			texts.at("ENEMYTEXT").setString("Dodged!");
		}
	}
	else
	{
		if (std::rand() % 100 > (pl.GetAttribute(ATRTYPES::dodgeChance) + 10))
		{ //enemy hit
			float enemyDamage = 0;
			float playerArmour = 0;
			float damageDealt = 0;
			if (std::rand() % 100 < en.GetAttribute(ATRTYPES::critChance) - 5)
			{ //enemy hit critical strike
				enemyDamage = static_cast<float>(en.GetAttribute(ATRTYPES::strenght)) * (static_cast<float>(en.GetAttribute(ATRTYPES::critDamage)) / 100) * 1.5;
				playerArmour = pl.GetAttribute(ATRTYPES::armour);
				damageDealt = enemyDamage - playerArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			else
			{ // enemy hit regural strike
				enemyDamage = static_cast<float>(en.GetAttribute(ATRTYPES::strenght)) * 1.5;
				playerArmour = pl.GetAttribute(ATRTYPES::armour);
				damageDealt = enemyDamage - playerArmour;
				if (damageDealt < 0)
				{
					damageDealt = 0;
				}
			}
			answerType = AnswerTypes::HURT;
			playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) = playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health) - damageDealt;
			texts.at("PLAYERHEALTH").setString("Health:" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth)));
		}
		else
		{ //enemy missed
			answerType = AnswerTypes::DODGE;
			texts.at("PLAYERTEXT").setString("Dodged!");
		}
	}
	switch (answerType)
	{
		case AnswerTypes::HURT:
			AudioPlayer::PlaySound("HURT");
			break;
		case AnswerTypes::DODGE:
			AudioPlayer::PlaySound("DODGE");
			break;
		case AnswerTypes::NOTHING:
			break;
		default:
			break;
	}
}

void CombatManager::AnswerRest()
{
	answerType = AnswerTypes::NOTHING;
}

void CombatManager::RegenerateHealth(bool isPlayer)
{
	if (isPlayer)
	{
		playerFighter.GetAttributeComponent().RegenerateHealth();
		texts.at("PLAYERHEALTH").setString(std::string("Health: " + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth))));
	}
	else
	{
		enemyFighter.GetAttributeComponent().RegenerateHealth();
		texts.at("ENEMYHEALTH").setString(std::string("Health: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::health)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxHealth))));
	}
}
void CombatManager::RegenerateStamina(bool isPlayer)
{
	if (isPlayer)
	{
		std::cout << "Player regenerating stamina" << std::endl;
		playerFighter.GetAttributeComponent().RegenerateStamina();
		texts.at("PLAYERSTAMINA").setString(std::string("Stamina: " + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(playerFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina))));
	}
	else
	{
		std::cout << "Enemy regenerating stamina" << std::endl;
		enemyFighter.GetAttributeComponent().RegenerateStamina();
		texts.at("ENEMYSTAMINA").setString(std::string("Stamina: " + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::stamina)) + "/" + std::to_string(enemyFighter.GetAttributeComponent().GetAttribute(ATRTYPES::maxStamina))));
	}
}

void CombatManager::SetResultItems()
{
	for (auto& a : enemy.itemDropList) //Loads items ids and amounts to give player
	{
		results.AddItemDrop(a);
	}

	for (auto& a : results.GetItemsDropped())
	{ //loads items to listbox
		inv.GiveItem(*itemTemplates.at(a.first), a.second);
		std::cout << "Gave item into droplist: " << itemTemplates.at(a.first)->GetName() << std::endl;
	}
}

bool CombatManager::GetFightResults()
{
	return endFight;
}
void CombatManager::DisplayGUI(sf::RenderTarget& trg)
{
	for (auto& a : btns)
	{
		a.second->Display(trg);
	}
	for (auto& a : displays)
	{
		a.second->Display(trg);
	}
	for (auto& a : descripts)
	{
		a.second->Display(trg);
	}

	for (auto& a : texts)
	{
		trg.draw(a.second);
	}

	playerInfo.Display(trg);
	enemyInfo.Display(trg);
	GUIResults.Display(trg);
}

void CombatManager::Display(sf::RenderTarget& trg)
{
	playerFighter.Display(trg);
	enemyFighter.Display(trg);
}

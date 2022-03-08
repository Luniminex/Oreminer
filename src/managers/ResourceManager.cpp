#include "managers/ResourceManager.hpp"

ResourceManager* ResourceManager::resourcemanager_ = nullptr;

std::map<std::string, std::string> ResourceManager::textures;
std::map<std::string, std::string> ResourceManager::data;
std::map<std::string, std::string> ResourceManager::maps;
std::map<std::string, std::string> ResourceManager::config;
std::map<std::string, std::string> ResourceManager::sounds;
std::map<std::pair<int, std::string>, std::string> ResourceManager::saveData;

ResourceManager::ResourceManager()
{
}

ResourceManager* ResourceManager::GetInstance()
{
	if (resourcemanager_ == nullptr)
	{
		resourcemanager_ = new ResourceManager();

		textures["PLAYERADVENTURESHEET"] = "content/Player/miner-spritesheet.png";			   //players texture in adventure state
		textures["PLAYERFIGHTSHEET"] = "content/Player/fightscene/player-animation-sheet.png"; //player texture in fightscene

		textures["MAPGROUNDTSHEET"] = "content/Map/ground.png";			  //texture of ground tiles
		textures["MAPOBJECTSHEET"] = "content/Objects/objectsheet.png";	  //texture sheet of objects t;hat map uses
		textures["MAPPORTALSHEET"] = "content/Objects/portalTilemap.png"; //texture sheet of portals that map uses

		textures["CAVEBGR"] = "content/backgrounds/cave.png";
		textures["FORESTBGR"] = "content/backgrounds/forest.png";

		data["OBJECTDATA"] = "content/Objects/objects.xml"; //xml file that contains data about objects which map uses
		data["PORTALDATA"] = "content/Map/portalList.xml";	//xml file that contains data about portals which map uses

		textures["ITEMSHEET"] = "content/Items/itemsheet.png"; //texture of item

		textures["ENEMYPREVIEW"] = "content/enemies/enemypreview.png";

		textures["ENEMYSHEET"] = "content/enemies/enemySheet-Adventure.png"; //texture sheet of all enemies that map uses

		textures["RIGHTSMALLARROW"] = "content/props/arrows/rightArrow.png";
		textures["BASICBTN"] = "content/props/GUI/buttons/basicButton.png";
		textures["BIGBTN"] = "content/props/GUI/buttons/bigBtn.png";
		textures["BGRBTN"] = "content/props/GUI/buttons/bgrBtn.png";
		textures["WIDEBTN"] = "content/props/GUI/buttons/wideBtn.png";

		data["ENEMYDATA"] = "content/enemies/enemies.xml"; //file containing data of enemies

		textures["NPCSHEET"] = "content/npcs/npcSheet-Adventure.png";

		data["NPCDATA"] = "content/npcs/npcs.xml";

		data["QUESTDATA"] = "content/npcs/quests.xml";

		data["PLAYERSAVE1ATTRIBUTES"] = "content/saves/save1/playerAttributes.xml"; //save 1, player attributes
		data["PLAYERSAVE1ITEMS"] = "content/saves/save1/playerItems.xml";			//save 1, player items

		data["ITEMDATA"] = "content/items/items.xml";
		data["RECIPEDATA"] = "content/npcs/craftingInterfaces.xml";

		data["STATISTICS"] = "content/saves/save1/playerStatistics.xml";

		maps["STARTINGMAP"] = "content/Map/forest.txt"; //1st map - starting area
		maps["MINESHAFTMAP"] = "content/Map/cave.txt";	//2nd map - mineshaft area

		config["SUPPORTEDKEYS"] = "content/config/supportedKeys.ini";

		config["MAINMENUKEYBINDS"] = "content/config/mainMenuState.ini";
		config["EDITORSTATEKEYBINDS"] = "content/config/editorStateKeybinds.ini";
		config["GAMESTATEKEYBINDS"] = "content/config/gameStateKeybinds.ini";
		config["FIGHTSTATEKEYBINDS"] = "content/config/fightStateKeybinds.ini";
		config["EDITORSTATEKEYBINDS"] = "content/config/editorStateKeybinds.ini";
		config["SETTINGSSTATEKEYBINDS"] = "content/config/settingsStateKeybinds.ini";
		config["WINDOWSETTINGS"] = "content/config/window.ini";

		config["ROBOTOFONT"] = "content/Fonts/Roboto-Medium.ttf";

		sounds["NEXTDIALOGUE"] = "content/Sound/SFX/dialogueNext.wav";
		sounds["MINE"] = "content/Sound/SFX/mine.wav";
		sounds["GETQUEST"] = "content/Sound/SFX/getQuest.wav";
		sounds["OREMINED"] = "content/Sound/SFX/oreMined.wav";
		sounds["WALKING"] = "content/Sound/SFX/walking.wav";
		sounds["BTNCLICK"] = "content/Sound/SFX/btnClick.wav";
		sounds["HURT"] = "content/Sound/SFX/hurt.wav";
		sounds["DODGE"] = "content/Sound/SFX/dodge.wav";
		sounds["LEVELUP"] = "content/Sound/SFX/levelUP.wav";
		sounds["QUICKSTRIKE"] = "content/Sound/SFX/quickstrike.wav";
		sounds["SLASH"] = "content/Sound/SFX/slash.wav";

		sounds["ADVENTURETHEME"] = "content/Sound/music/adventureTheme.wav";
		sounds["MAINMENUTHEME"] = "content/Sound/music/mainMenuTheme.wav";
		sounds["FORESTTHEME"] = "content/Sound/music/forestTheme.wav";
		sounds["CAVETHEME"] = "content/Sound/music/caveTheme.wav";
		sounds["CAVETHEMESTART"] = "content/Sound/music/caveThemeStart.wav";
		sounds["CAVETHEMELOOP"] = "content/Sound/music/caveThemeLoop.wav";
		sounds["FORESTTHEMESTART"] = "content/Sound/music/forestThemeStart.wav";
		sounds["FORESTTHEMELOOP"] = "content/Sound/music/forestThemeLoop.wav";

		saveData[std::pair(1, "EDITORKEYBINDS")] = "content/saves/save1/config/editorStateKeybinds.ini";
		saveData[std::pair(1, "FIGHTKEYBINDS")] = "content/saves/save1/config/fightStateKeybinds.ini";
		saveData[std::pair(1, "GAMEKEYBINDS")] = "content/saves/save1/config/gameStateKeybinds.ini";
		saveData[std::pair(1, "SETTINGSKEYBINDS")] = "content/saves/save1/config/settingsStateKeybinds.ini";

		saveData[std::pair(1, "MINESHAFTMAP")] = "content/saves/save1/map/mineshaftArea.txt";
		saveData[std::pair(1, "STARTINGMAP")] = "content/saves/save1/map/startingArea.txt";

		saveData[std::pair(1, "PLAYERATRRIBUTES")] = "content/saves/save1/player/playerAttributes.xml";
		saveData[std::pair(1, "PLAYERITEMS")] = "content/saves/save1/player/playerItems.xml";
		saveData[std::pair(1, "PLAYERSTATISTICS")] = "content/saves/save1/player/playerStatistics.xml";

		saveData[std::pair(1, "QUESTS")] = "content/saves/save1/quests/quests.xml";

		saveData[std::pair(2, "EDITORKEYBINDS")] = "content/saves/save2/config/editorStateKeybinds.ini";
		saveData[std::pair(2, "FIGHTKEYBINDS")] = "content/saves/save2/config/fightStateKeybinds.ini";
		saveData[std::pair(2, "GAMEKEYBINDS")] = "content/saves/save2/config/gameStateKeybinds.ini";
		saveData[std::pair(2, "SETTINGSKEYBINDS")] = "content/saves/save2/config/settingsStateKeybinds.ini";

		saveData[std::pair(2, "MINESHAFTMAP")] = "content/saves/save2/map/mineshaftArea.txt";
		saveData[std::pair(2, "STARTINGMAP")] = "content/saves/save2/map/startingArea.txt"; /* NEFUNGUJE INPUT V GAMESTATE U SAVE 2 PAUSE*/

		saveData[std::pair(2, "PLAYERATRRIBUTES")] = "content/saves/save2/player/playerAttributes.xml";
		saveData[std::pair(2, "PLAYERITEMS")] = "content/saves/save2/player/playerItems.xml";

		saveData[std::pair(2, "PLAYERSTATISTICS")] = "content/saves/save2/player/playerStatistics.xml";

		saveData[std::pair(2, "QUESTS")] = "content/saves/save2/quests/quests.xml";

		saveData[std::pair(3, "EDITORKEYBINDS")] = "content/saves/save3/config/editorStateKeybinds.ini";
		saveData[std::pair(3, "FIGHTKEYBINDS")] = "content/saves/save3/config/fightStateKeybinds.ini";
		saveData[std::pair(3, "GAMEKEYBINDS")] = "content/saves/save3/config/gameStateKeybinds.ini";
		saveData[std::pair(3, "SETTINGSKEYBINDS")] = "content/saves/save3/config/settingsStateKeybinds.ini";

		saveData[std::pair(3, "MINESHAFTMAP")] = "content/saves/save3/map/mineshaftArea.txt";
		saveData[std::pair(3, "STARTINGMAP")] = "content/saves/save3/map/startingArea.txt"; /* NEFUNGUJE INPUT V GAMESTATE U SAVE 2 PAUSE*/

		saveData[std::pair(3, "PLAYERATRRIBUTES")] = "content/saves/save3/player/playerAttributes.xml";
		saveData[std::pair(3, "PLAYERITEMS")] = "content/saves/save3/player/playerItems.xml";

		saveData[std::pair(3, "PLAYERSTATISTICS")] = "content/saves/save3/player/playerStatistics.xml";

		saveData[std::pair(3, "QUESTS")] = "content/saves/save3/quests/quests.xml";
	}
	return resourcemanager_;
}

std::string& ResourceManager::TextureAt(const std::string& value)
{
	if (textures.find(value) == textures.end())
	{
		//std::cout << "RESOURCEMANAGER::TEXTUREAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
	else
	{
		//std::cout << "RESOUCEMANAGER::TEXTUREAT::key:" << value << " value:" << textures.at(value) << std::endl;
	}
	return textures.at(value);
}

std::string& ResourceManager::DataAt(const std::string& value)
{
	if (data.find(value) == data.end())
	{
		//std::cout << "RESOURCEMANAGER::DATAAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
	else
	{
		//std::cout << "RESOUCEMANAGER::DATAAT::key:" << value << " value:" << data.at(value) << std::endl;
	}
	return data.at(value);
}

std::string& ResourceManager::MapAt(const std::string& value)
{
	if (maps.find(value) == maps.end())
	{
		//std::cout << "RESOURCEMANAGER::MAPAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
	else
	{
		//std::cout << "RESOUCEMANAGER::MAPAT::key:" << value << " value:" << maps.at(value) << std::endl;
	}
	return maps.at(value);
}

std::string& ResourceManager::ConfigAt(const std::string& value)
{
	if (config.find(value) == config.end())
	{
		//std::cout << "RESOURCEMANAGER::CONFIGAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
	else
	{
		//std::cout << "RESOUCEMANAGER::CONFIGAT::key:" << value << " value:" << config.at(value) << std::endl;
	}
	return config.at(value);
}

std::string& ResourceManager::SoundAt(const std::string& value)
{
	if (sounds.find(value) == sounds.end())
	{
		//std::cout << "RESOURCEMANAGER::SOUNDSAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
	else
	{
		//std::cout << "RESOUCEMANAGER::SOUNDSAT::key:" << value << " value:" << sounds.at(value) << std::endl;
	}
	return sounds.at(value);
}

std::string& ResourceManager::SaveDataAt(const int save, const std::string& value)
{

	try
	{
		//std::cout << "RESOUCEMANAGER::CONFIGAT::key:" << value << " value:" << config.at(value) << std::endl;
		return saveData.at(std::pair(save, value));
	}
	catch (int temp)
	{
		//std::cout << "RESOURCEMANAGER::SAVEDATAAT:: NO VALUE AT KEY: " << value << std::endl;
		error = value.c_str();
		return error;
	}
}

std::map<std::string, std::string>& ResourceManager::GetMaps()
{
	return maps;
}
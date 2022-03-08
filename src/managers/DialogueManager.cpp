#include "DialogueManager.hpp"

DialogueManager* DialogueManager::dialogueManager_ = nullptr;

DialogueManager::DialogueManager()
{

	rapidxml::xml_document doc;
	rapidxml::xml_node<>* node;

	std::ifstream inFile("content/npcs/dialogues.xml");
	if (!inFile.is_open())
	{
		std::cout << "FAILED TO LOAD DIALOGUES PATHS" << std::endl;
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	node = doc.first_node("dialogues")->first_node("dialogue");

	const char* temp = nullptr;

	Dialogue tempDialogue;

	int npcid = 0;
	int version = 0;
	std::string page;
	std::string nameOfPage;

	std::map<std::string, int> strMap;
	enum strings
	{
		npcID,
		Eversion,
		name,
		tempPage

	};
	strMap["npcid"] = strings::npcID;
	strMap["version"] = strings::Eversion;
	strMap["name"] = strings::name;
	strMap["page"] = strings::tempPage;

	for (rapidxml::xml_node<>* parent = node; parent; parent = parent->next_sibling())
	{
		for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling())
		{
			switch (strMap.at(child->name()))
			{
				case strings::npcID:
					temp = child->value();
					npcid = std::stoi(std::string(temp));
					break;
				case strings::Eversion:
					temp = child->value();
					version = std::stoi(std::string(temp));
					break;
				case strings::name:
					nameOfPage = std::string(child->value());
					break;
				case strings::tempPage:
					page = std::string(child->value());
					tempDialogue.pages.push_back(page);
					break;
				default:
					break;
			}
		}
		npcDialogueMap[std::pair(npcid, nameOfPage)].push_back(version);

		dialogues[std::pair(npcid, std::pair(nameOfPage, version))] = tempDialogue;
		tempDialogue.pages.clear();
	}
	doc.clear();
	delete temp;
}

DialogueManager* DialogueManager::GetInstance()
{
	if (dialogueManager_ == nullptr)
	{
		dialogueManager_ = new DialogueManager();
	}

	return dialogueManager_;
}

/********accessors********/

Dialogue& DialogueManager::GetDialogue(int npcID, std::string dialogueName, int version)
{

	return dialogues.at(std::pair(npcID, std::pair(dialogueName, version)));
}

std::vector<int>& DialogueManager::GetNPCDialogueVersions(int npcID, const std::string& dialogueType)
{
	return npcDialogueMap.at(std::pair(npcID, dialogueType));
}

int DialogueManager::GetRandomVersionDialogue(int npcID, const std::string& type)
{

	std::vector<int>& temp(npcDialogueMap.at(std::pair(npcID, type)));
	return temp.at(rand() % temp.size());
}
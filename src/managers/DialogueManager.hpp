#ifndef DIALOGUEMANAGER_HPP
#define DIALOGUEMANAGER_HPP

#include "managers/ResourceManager.hpp"
#include <rapidxml-1.13/rapidxml_ext.hpp>

enum DialoguesType
{
	welcome,
	giveQuest,
	completeQuest
};

struct Dialogue
{
	std::vector<std::string> pages;
};

class DialogueManager
{

private:
	static DialogueManager* dialogueManager_;

public:
	//id of npc, name of dialogue, version of the dialogue
	std::map<std::pair<int, std::pair<std::string, int>>, Dialogue> dialogues;

	//npc id, type of dialogue, list of version of that dialogue
	std::map<std::pair<int, std::string>, std::vector<int>> npcDialogueMap;

private:
	DialogueManager();

	//singleton
public:
	static DialogueManager* GetInstance();

	~DialogueManager();

	DialogueManager(DialogueManager& copy) = delete;

	void operator=(const DialogueManager&) = delete;

	//accessors
public:
	Dialogue& GetDialogue(int npcID, std::string dialogueName, int version);
	std::vector<int>& GetNPCDialogueVersions(int npcID, const std::string& dialogueType);
	int GetRandomVersionDialogue(int npcID, const std::string& type);
};

#endif
#ifndef DIALOGUECOMPONENT_HPP
#define DIALOGUECOMPONENT_HPP

#include "GUI/GUI.hpp"
#include "managers/DialogueManager.hpp"

class DialogueComponent
{

private:
	GUI::DialogueBox dialogueBox;
	DialogueManager* dialogueman = DialogueManager::GetInstance();

	bool alreadyFinished = true;

	//constructor / destructor
public:
	DialogueComponent(sf::Font& font, sf::Vector2f res);
	~DialogueComponent();

	//accessors
public:
	bool IsFinished();

	//modifiers
public:
	void Show(bool shown);

	void NextDialogue();

	void SetDialogue(int npcID, const std::string& type, int version);

	//public functions
public:
	bool Update();

	void Display(sf::RenderTarget& trg);
	//private functions
private:
};

#endif

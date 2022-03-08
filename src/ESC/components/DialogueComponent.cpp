#include "DialogueComponent.hpp"

/************constructor / destructor**********/

DialogueComponent::DialogueComponent(sf::Font& font, sf::Vector2f res) :
	dialogueBox(font, res)
{
}

DialogueComponent::~DialogueComponent()
{
}

/********accessors***********/

bool DialogueComponent::IsFinished()
{
	if (alreadyFinished == false && dialogueBox.IsFinished() == true)
	{
		alreadyFinished = true;
		return true;
	}
	else
	{
		return false;
	}
}

/***********modfiers***********/

void DialogueComponent::Show(bool shown)
{
	dialogueBox.Show(shown);
}

void DialogueComponent::NextDialogue()
{
	dialogueBox.NextDialogue();
}

void DialogueComponent::SetDialogue(int npcID, const std::string& type, int version)
{

	dialogueBox.SetDialogue(&dialogueman->GetDialogue(npcID, type, version).pages);
	dialogueBox.shown = true;
	alreadyFinished = false;
}

/***********public functions***************/

bool DialogueComponent::Update()
{
	if(dialogueBox.Update())
	{
		return true;
	}
	return false;
}

void DialogueComponent::Display(sf::RenderTarget& trg)
{
	dialogueBox.Display(trg);
}

/***********private functions***************/

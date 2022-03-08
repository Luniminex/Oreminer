#include "GUI/GUI.hpp"

/***********constructor / destructor*************/

GUI::DialogueBox::DialogueBox(sf::Font& font, sf::Vector2f res) :
	TexturedGUI(),
	dialogue(),
	box(),
	next(),
	tex()
{
	texts = nullptr;
	textureType = btnTypes::wide;
	textured = true;

	resourceman = ResourceManager::GetInstance();

	shown = true;
	nextPage = false;
	page = 0;
	finished = false;

	box.setSize(sf::Vector2f(res.x / 2, res.y / 7));
	box.setPosition(res.x / 4.f, res.y - res.y / 6);
	box.setTexture(&btnTextures.at(textureType));
	sf::IntRect temp;
	temp.left = 0;
	temp.top = 0;
	temp.height = btnTextures.at(textureType).getSize().y;
	temp.width = btnTextures.at(textureType).getSize().x;
	box.setTextureRect(temp);
	dialogue.setFont(font);
	dialogue.setCharacterSize(GUI::CaclCharSize(res));
	dialogue.setString("");
	dialogue.setString(GUI::FitTextInsideBox(dialogue, box.getSize().x));
	dialogue.setPosition(box.getPosition() + sf::Vector2f(50.f, 10.f));

	if (!tex.loadFromFile(resourceman->TextureAt("RIGHTSMALLARROW")))
	{
		std::cout << "GUI::DIALOGUE BOX:: COULD NOT LOAD ARROW TEXTURE" << std::endl;
	}

	next.setTexture(&tex);
	next.setSize(sf::Vector2f(32.f, 32.f));
	next.setScale(sf::Vector2f(2.f, 2.f));
	next.setPosition(box.getPosition().x + box.getGlobalBounds().width - next.getSize().x * 2, box.getPosition().y + box.getGlobalBounds().height - next.getSize().y * 2);
}

GUI::DialogueBox::~DialogueBox()
{
}
/***********accessors*************/

bool GUI::DialogueBox::IsShown()
{
	return shown;
}

bool GUI::DialogueBox::IsFinished()
{
	if (finished == true)
	{
		std::cout << "DIALOGUECOMP:: FINISHED DIALOGUE TRUE" << std::endl;
	}
	return finished;
}
/***********modifiers*************/

void GUI::DialogueBox::Show(bool shown)
{
	this->shown = shown;
}

void GUI::DialogueBox::NextDialogue()
{
	nextPage = true;
}

void GUI::DialogueBox::SetDialogue(std::vector<std::string>* texts)
{
	if (texts == nullptr)
	{
		std::cout << "Dialogue is nullptr" << std::endl;
		return;
	}

	this->texts = texts;

	page = 0;
	dialogue.setString(texts->at(page));
}

/***********public functions*************/
void GUI::DialogueBox::AfterInit(sf::Vector2f res)
{
	(void)res;
}

bool GUI::DialogueBox::Update()
{
	box.setTexture(&btnTextures.at(textureType));
	sf::IntRect temp;
	temp.left = 0;
	temp.top = 0;
	temp.height = btnTextures.at(textureType).getSize().y;
	temp.width = btnTextures.at(textureType).getSize().x;
	box.setTextureRect(temp);
	if (shown && nextPage)
	{
		if (texts)
		{
			if (static_cast<unsigned>(page) < texts->size())
			{
				finished = false;
				dialogue.setString(texts->at(page));
				dialogue.setString(GUI::FitTextInsideBox(dialogue, box.getSize().x));
				nextPage = false;
			}
			else
			{
				shown = false;
				finished = true;
			}
			page++;
			return true;
		}
	}
	return false;
}

void GUI::DialogueBox::Display(sf::RenderTarget& trg)
{
	if (shown)
	{
		trg.draw(box);
		trg.draw(dialogue);
		trg.draw(next);
	}
}

/***********private functions*************/
#include "GUI/GUI.hpp"

/***********constructor / destructor**********/

GUI::AttributeInfo::AttributeInfo(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res) :
	TexturedGUI(),
	font(font)
{
	this->keyTimeMax = 10.f;
	this->keyTime = 0.f;
	textured = true;
	textureType = btnTypes::bgr;
	shown = false;

	this->posPerc = posPerc;
	this->sizePerc = sizePerc;

	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	shape.setSize(size);
	shape.setPosition(position);
	shape.setTexture(&btnTextures.at(textureType));
	sf::IntRect tempRect;
	tempRect.left = 0;
	tempRect.top = 0;
	tempRect.height = btnTextures.at(textureType).getSize().y;
	tempRect.width = btnTextures.at(textureType).getSize().x;
	shape.setTextureRect(tempRect);

	text.setFont(font);
	text.setCharacterSize(CaclCharSizeSmall(res));
	text.setPosition(position);
}

GUI::AttributeInfo::~AttributeInfo()
{
}

/***********accessors************/
bool GUI::AttributeInfo::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

bool GUI::AttributeInfo::IsShown()
{
	return shown;
}
/***********modifiers**********/

void GUI::AttributeInfo::ClearAttributes()
{
	text.setString("");
	shape.setSize(sf::Vector2f(0.f, 0.f));
}

void GUI::AttributeInfo::Show()
{
	shown = true;
}

void GUI::AttributeInfo::Hide()
{
	shown = false;
}

void GUI::AttributeInfo::AddAttribute(const std::string& str)
{
	text.setString(text.getString() + "\n" + str);
	shape.setSize(sf::Vector2f(text.getGlobalBounds().width + 20.f, text.getGlobalBounds().height + 30.f));
	shape.setPosition(sf::Vector2f(text.getGlobalBounds().left - 15.f, text.getGlobalBounds().top - 10.f));
}

/**********public functions************/

void GUI::AttributeInfo::AfterInit(sf::Vector2f res)
{
	this->currentResolution = res;
	sf::Vector2f pos = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	//sf::Vector2f size = {PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y)};

	this->text.setCharacterSize(CaclCharSizeSmall(res));
	this->text.setPosition(pos);

	shape.setSize(sf::Vector2f(text.getGlobalBounds().width + 20.f, text.getGlobalBounds().height + 30.f));
	shape.setPosition(sf::Vector2f(text.getGlobalBounds().left - 15.f, text.getGlobalBounds().top - 10.f));
}

void GUI::AttributeInfo::Update(float dt)
{
	UpdateKeyTimer(dt);
}

void GUI::AttributeInfo::Display(sf::RenderTarget& trg)
{
	if (shown)
	{
		trg.draw(shape);
		trg.draw(text);
	}
}

/**********private functions**********/

void GUI::AttributeInfo::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}
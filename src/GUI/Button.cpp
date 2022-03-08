#include "GUI/GUI.hpp"

//constructor / desctructor

GUI::Button::Button(float posXPerc, float posYPerc, sf::Vector2f sizeInPerc,
	sf::Font& font, const std::string& displayText, sf::Color idleClr, sf::Color hoverClr,
	sf::Color pressedClr, sf::Vector2f res) :
	TexturedGUI(),
	font(font)
{

	std::cout << "Creating a " << displayText << std::endl;
	InitBtn(posXPerc, posYPerc, sizeInPerc, font, displayText, res);
	this->body.setFillColor(idleClr);

	this->idleClr = idleClr;
	this->hoverClr = hoverClr;
	this->pressedClr = pressedClr;
	this->state = btnStates::BTN_IDLE;
	this->idleOutlClr = sf::Color::Transparent;
	this->hoverOutlClr = sf::Color::Transparent;
	this->pressedOutlClr = sf::Color::Transparent;
	textured = false;
}

GUI::Button::Button(float posXPerc, float posYPerc, sf::Vector2f sizeInPerc, sf::Font& font, const std::string& displayText, int textureBtnType, sf::Vector2f res) :
	font(font)
{
	InitBtn(posXPerc, posYPerc, sizeInPerc, font, displayText, res);
	body.setTexture(&btnTextures.at(textureBtnType));
	sf::IntRect temp;
	temp.left = 0;
	temp.top = 0;
	temp.height = btnTextures.at(textureBtnType).getSize().y / 3;
	temp.width = btnTextures.at(textureBtnType).getSize().x;
	body.setTextureRect(temp);
	textured = true;
	textureType = textureBtnType;
}

GUI::Button::Button(const Button& btn) :
	font(btn.font)
{

	this->body = btn.body;

	this->text = btn.text;

	this->idleClr = idleClr;
	this->hoverClr = hoverClr;
	this->pressedClr = pressedClr;

	this->idleOutlClr = sf::Color::Transparent;
	this->hoverOutlClr = sf::Color::Transparent;
	this->pressedOutlClr = sf::Color::Transparent;

	this->state = btnStates::BTN_IDLE;

	shown = true;
	textured = false;
}

GUI::Button::~Button()
{
}

void GUI::Button::InitBtn(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
	sf::Font& font, const std::string& displayText, sf::Vector2f res)
{
	this->posPerc = { posXPerc, posYPerc };
	this->sizePerc = sizePerc; //saves values

	float posX = PeToPiX(posPerc.x, res.x); //converts percentages to position
	float posY = PeToPiY(posPerc.y, res.y);

	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) }; // converts size in perc to actual size

	this->body.setSize(size);
	this->body.setPosition(posX, posY);

	this->text.setFont(font);
	this->text.setString(displayText);
	this->text.setCharacterSize(CaclCharSize(res));

	this->text.setPosition(0.f, 0.f);

	this->text.setPosition(sf::Vector2f(
		this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));

	shown = true;
}

//accessors

sf::Vector2f GUI::Button::GetPos()
{
	return this->body.getPosition();
}

sf::FloatRect GUI::Button::GetGlobalBounds()
{
	return this->body.getGlobalBounds();
}

sf::Vector2f GUI::Button::GetSize()
{
	return this->body.getSize();
}

unsigned int GUI::Button::GetState()
{
	return state;
}

const std::string GUI::Button::GetText()
{
	return this->text.getString();
}

bool GUI::Button::IsShown()
{
	return shown;
}

/************************************modifiers***************************************************/

void GUI::Button::SetPosition(sf::Vector2f pos)
{
	this->body.setPosition(pos);
	this->text.setPosition(sf::Vector2f(
		this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));
}

void GUI::Button::SetSize(float x, float y)
{
	this->body.setSize(sf::Vector2f(x, y));
}

void GUI::Button::SetCharSize(unsigned int size)
{
	this->text.setCharacterSize(size);
}

/**********************************functions*************************************/

void GUI::Button::AferInit(sf::Vector2f res)
{
	SetCharSize(CaclCharSize(res));
	SetSize(PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y));
	SetPosition(PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y));
}

void GUI::Button::Update(const sf::Vector2i& mousePosWindow)
{
	if (shown)
	{								 //pokud je tlačítko zobrazeno
		state = btnStates::BTN_IDLE; //nastav stav tlačítka na nečinný

		if (body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{								  //pokud je kurzor v oblasti tlačítka
			state = btnStates::BTN_HOVER; //nastav stav tlačítka na vznášení
			if (textured)
			{
				this->text.setPosition(sf::Vector2f(
					this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
					this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));
				sf::IntRect temp;
				temp.left = 0;
				temp.top = btnTextures.at(textureType).getSize().y / 3;
				temp.height = btnTextures.at(textureType).getSize().y / 3;
				temp.width = btnTextures.at(textureType).getSize().x;
				body.setTextureRect(temp);
				text.setPosition(text.getPosition().x, text.getPosition().y + 2.5f);
			}
			else
			{
				body.setFillColor(hoverClr);		//nastaví barvu výpně na vznášení
				body.setOutlineColor(hoverOutlClr); //nastaví barvu obrysu na vznášení
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{									//je tlačítko na myši stisknuto
				state = btnStates::BTN_PRESSED; //nastavení stavu na zmáčknuto
				if (textured)
				{
					this->text.setPosition(sf::Vector2f(
						this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
						this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));
					sf::IntRect temp;
					temp.left = 0;
					temp.top = (btnTextures.at(textureType).getSize().y / 3) * 2;
					temp.height = btnTextures.at(textureType).getSize().y / 3;
					temp.width = btnTextures.at(textureType).getSize().x;
					body.setTextureRect(temp);
					text.setPosition(text.getPosition().x, text.getPosition().y + 5.f);
				}
				else
				{
					body.setFillColor(pressedClr);		  //nastavení barvy výplně na zmáčknuto
					body.setOutlineColor(pressedOutlClr); //nastavení barvy obrysu na zmáčknuto
				}
				AudioPlayer::PlaySound("BTNCLICK"); //přehraje se zvukový efekt zmáčknutí tlačítka
			}
		}
		else
		{ //kurzor není v oblasti tlačítka
			if (textured)
			{
				this->text.setPosition(sf::Vector2f(
					this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
					this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));
				sf::IntRect temp;
				temp.left = 0;
				temp.top = 0;
				temp.height = btnTextures.at(textureType).getSize().y / 3;
				temp.width = btnTextures.at(textureType).getSize().x;
				body.setTextureRect(temp);
			}
			else
			{
				body.setFillColor(idleClr);		   //nastavení výplně na nečinnou
				body.setOutlineColor(idleOutlClr); //nastavení obysu na nečinnou
			}
		}
	}
}

void GUI::Button::SetPosition(float x, float y)
{
	this->body.setPosition(sf::Vector2f(x, y));
	this->text.setPosition(sf::Vector2f(
		this->body.getPosition().x + (this->body.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->body.getPosition().y + (this->body.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2.f)));
}

void GUI::Button::SetText(const std::string& str)
{
	this->text.setString(str);
}

void GUI::Button::setOutline(float outlineThickness, sf::Color idleOutlClr,
	sf::Color hoverOutlClr, sf::Color pressedOutlClr)
{
	this->body.setOutlineThickness(outlineThickness);
	this->idleOutlClr = idleOutlClr;
	this->hoverOutlClr = hoverOutlClr;
	this->pressedOutlClr = pressedOutlClr;
}

void GUI::Button::SetShown(bool shown)
{
	this->shown = shown;
	if (shown == false)
	{
		this->state = btnStates::BTN_IDLE;
	}
}

void GUI::Button::Display(sf::RenderTarget& target)
{
	if (shown)
	{
		target.draw(body);
		target.draw(text);
	}
}
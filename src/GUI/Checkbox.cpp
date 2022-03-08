#include "GUI/GUI.hpp"

//constructor / destructor
GUI::Checkbox::Checkbox(float posXPerc, float posYPerc, sf::Vector2f sizePerc, float offsetPerc, bool checked,
	sf::Font& font, const std::string& displayText, float outThickness, sf::Color idleCheckedColor,
	sf::Color idleUncheckedColor, sf::Color hoverCheckedColor, sf::Color hoverUncheckedColor,
	sf::Color PressedCheckedColor, sf::Color PressedUncheckedColor, sf::Color idleCheckedColorOut,
	sf::Color idleUncheckedColorOut, sf::Color hoverCheckedColorOut, sf::Color hoverUncheckedColorOut,
	sf::Color PressedCheckedColorOut, sf::Color PressedUncheckedColorOut, sf::Vector2f res) :
	font(font)
{
	(void)res;

	this->posPerc = { posXPerc, posYPerc };
	this->sizePerc = sizePerc;
	this->offsetPerc = offsetPerc;

	float posX = PeToPiX(posXPerc, res.x);
	float posY = PeToPiY(posYPerc, res.y);
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };
	float offset = PeToPiX(offsetPerc, res.x);

	this->state = btnStates::BTN_IDLE;
	this->checked = checked;

	this->body.setSize(size);
	this->body.setOutlineThickness(outThickness);

	if (checked)
	{
		this->body.setFillColor(idleCheckedColor);
		this->body.setOutlineColor(idleCheckedColorOut);
	}
	else
	{
		this->body.setFillColor(idleUncheckedColor);
		this->body.setOutlineColor(idleUncheckedColorOut);
	}

	this->idleCheckedColor = idleCheckedColor;
	this->idleUncheckedColor = idleUncheckedColor;

	this->hoverCheckedColor = hoverCheckedColor;
	this->hoverUncheckedColor = hoverUncheckedColor;

	this->PressedCheckedColor = PressedCheckedColor;
	this->PressedUncheckedColor = PressedUncheckedColor;

	this->idleCheckedColorOut = idleCheckedColorOut;
	this->idleUncheckedColorOut = idleUncheckedColorOut;

	this->hoverCheckedColorOut = hoverCheckedColorOut;
	this->hoverUncheckedColorOut = hoverUncheckedColorOut;

	this->PressedCheckedColorOut = PressedCheckedColorOut;
	this->PressedUncheckedColorOut = PressedUncheckedColorOut;

	this->text.setFont(font);
	this->text.setCharacterSize(CaclCharSize(res));
	this->text.setString(displayText);
	this->text.setPosition(posX, posY);

	this->body.setPosition(sf::Vector2f(
		text.getGlobalBounds().left + text.getGlobalBounds().width + offset,
		text.getGlobalBounds().top - (body.getGlobalBounds().height / 4.f)));

	keyTime = 0.f;
	keyTimeMax = 13.f;
}

GUI::Checkbox::~Checkbox()
{
}

//accessors
unsigned int GUI::Checkbox::GetState()
{
	return state;
}

bool GUI::Checkbox::Checked()
{
	return checked;
}
//modifiers
void GUI::Checkbox::SetPosition(float x, float y, float offset)
{
	this->text.setPosition(x, y);
	this->body.setPosition(sf::Vector2f(
		text.getGlobalBounds().left + text.getGlobalBounds().width + offset,
		text.getGlobalBounds().top));
}

void GUI::Checkbox::SetSize(float x, float y)
{
	this->body.setSize(sf::Vector2f(x, y));
}

void GUI::Checkbox::SetCharSize(unsigned int size)
{
	this->text.setCharacterSize(size);
}

void GUI::Checkbox::SetText(const std::string& str)
{
	this->text.setString(str);
}

/******************************functions***********************************/

void GUI::Checkbox::AferInit(sf::Vector2f res)
{
	SetCharSize(CaclCharSize(res));
	SetSize(PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y));
	SetPosition(PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y), PeToPiX(offsetPerc, res.x));
}

void GUI::Checkbox::Update(const sf::Vector2i& mousePosWindow, float dt)
{
	UpdateKeyTimer(dt);

	state = btnStates::BTN_IDLE;
	if (checked)
	{
		body.setFillColor(idleCheckedColor);
		body.setOutlineColor(idleCheckedColorOut);
	}
	else
	{
		body.setFillColor(idleUncheckedColor);
		body.setOutlineColor(idleUncheckedColorOut);
	}

	if (body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
	{
		state = btnStates::BTN_HOVER;
		if (checked)
		{
			body.setFillColor(hoverCheckedColor);
			body.setOutlineColor(hoverCheckedColorOut);
		}
		else
		{
			body.setFillColor(hoverUncheckedColor);
			body.setOutlineColor(hoverUncheckedColorOut);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && GetKeyTime() == true)
		{
			state = btnStates::BTN_PRESSED;
			checked = !checked;
			if (checked)
			{
				body.setFillColor(PressedCheckedColor);
				body.setOutlineColor(PressedCheckedColorOut);
				AudioPlayer::PlaySound("BTNCLICK");
			}
			else
			{
				body.setFillColor(PressedUncheckedColor);
				body.setOutlineColor(PressedUncheckedColorOut);
				AudioPlayer::PlaySound("BTNCLICK");
			}
		}
	}
}

void GUI::Checkbox::Display(sf::RenderTarget& target)
{
	target.draw(body);
	target.draw(text);
}

//private functions

void GUI::Checkbox::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

bool GUI::Checkbox::GetKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

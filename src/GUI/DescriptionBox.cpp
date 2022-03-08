#include "GUI/GUI.hpp"

/**********************constructors / destructors**********************/

GUI::DescriptionBox::DescriptionBox(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Texture& tex, sf::IntRect texRect, sf::Font& font,
	ItemData& data, sf::Vector2f res) :
	TexturedGUI(),
	font(font),
	data(data)
{

	shown = false;
	textureType = btnTypes::bgr;
	textured = true;
	NOBtns = 1;

	keyTime = 0.f;
	keyTimeMax = 13.f;

	this->currentRes = res;

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

	img.setTexture(&tex);
	img.setTextureRect(texRect);
	img.setSize(sf::Vector2f(size.y / 6, size.y / 6)); //all y or x so its uniform
	img.setPosition(sf::Vector2f(position.x + size.x * 0.05f, position.y + size.y * 0.05f));

	this->title.setFont(font);
	this->description.setFont(font);
	this->leftSpecial.setFont(font);
	this->rightSpecial.setFont(font);

	this->title.setString(data.name);
	this->description.setString(data.name);
	this->leftSpecial.setString(std::string("Amount:" + std::to_string(data.count)));
	if (data.equiped)
	{
		this->rightSpecial.setString(std::string("Equiped"));
	}
	else
	{
		this->rightSpecial.setString(std::string(""));
	}

	this->title.setCharacterSize(CaclCharSize(res));
	this->description.setCharacterSize(CaclCharSizeSmall(res));
	this->leftSpecial.setCharacterSize(CaclCharSizeSmall(res));
	this->rightSpecial.setCharacterSize(CaclCharSizeSmall(res));

	this->title.setPosition(img.getGlobalBounds().left + img.getGlobalBounds().width + ((size.x - (img.getGlobalBounds().width + size.x * 0.05f)) / 2) - this->title.getGlobalBounds().width, position.y + this->title.getGlobalBounds().height + size.y * 0.05);

	this->description.setString(FitTextInsideBox(this->description, (size.x / 10) * 8));
	//x - size needs to be divided by 2x the amount that the boundary is
	this->description.setPosition(position.x + (size.x / 20), this->title.getGlobalBounds().top + this->title.getGlobalBounds().height + size.y * 0.1);

	btns.resize(maxNOBtns);

	btns.at(0) = std::unique_ptr<GUI::Button>(new GUI::Button(posPerc.x, PiToPeY(position.y + shape.getGlobalBounds().height, res.y), sf::Vector2f(PiToPeX(size.x / 5, res.x), PiToPeY(size.y / 5, res.y)), font, "Close", btnTypes::basic, res));
	btns.at(0)->SetPosition(position.x, position.y + size.y - btns.at(0)->GetGlobalBounds().height);

	this->leftSpecial.setPosition(position.x + (size.x * 0.05), position.y + size.y - btns.at(0)->GetGlobalBounds().height - (this->leftSpecial.getGlobalBounds().height * 2));

	this->rightSpecial.setPosition(position.x + size.x - this->rightSpecial.getGlobalBounds().width - (size.x * 0.05), position.y + size.y - (this->rightSpecial.getGlobalBounds().height * 2) - btns.at(0)->GetGlobalBounds().height);

	for (int i = 1; i < maxNOBtns; i++)
	{
		btns.at(i) = nullptr;
	};
}

GUI::DescriptionBox::~DescriptionBox()
{
}

/**********************accessors**********************/

bool GUI::DescriptionBox::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

bool GUI::DescriptionBox::IsShown()
{
	return shown;
}

GUI::Button& GUI::DescriptionBox::GetButton(unsigned int index)
{

	if (index > static_cast<unsigned int>(maxNOBtns)) // if index bigger than max amount of btns, return btn at 0 - default close btn
	{
		return *btns.at(0);
	}
	if (index >= static_cast<unsigned int>(NOBtns))
	{
		return *btns.at(0);
	}
	return *btns.at(index);
}

int GUI::DescriptionBox::GetNOBtns()
{
	return NOBtns;
}

ItemData& GUI::DescriptionBox::GetItemData()
{
	return data;
}

std::string GUI::DescriptionBox::GetTitle()
{
	return title.getString();
}

/**********************Modifiers**********************/
void GUI::DescriptionBox::Show(bool show)
{
	shown = show;
}

void GUI::DescriptionBox::Hide()
{
	shown = false;
}

bool GUI::DescriptionBox::AddButton(const std::string& text)
{
	if (NOBtns >= maxNOBtns)
	{
		return false;
	}
	else
	{
		btns.at(NOBtns) = std::unique_ptr<GUI::Button>(new GUI::Button(posPerc.x, posPerc.y, sf::Vector2f(sizePerc.x / 5, sizePerc.y / 5), font, text, btnTypes::basic, currentRes));
		btns.at(NOBtns)->SetPosition(PeToPiX(posPerc.x, currentRes.x) + btns.at(0)->GetGlobalBounds().width * NOBtns, PeToPiY(posPerc.y, currentRes.y) + PeToPiY(sizePerc.y, currentRes.y) - btns.at(0)->GetGlobalBounds().height);
		NOBtns++;
		return true;
	}
}

bool GUI::DescriptionBox::RemoveButton()
{
	if (NOBtns > 1)
	{
		NOBtns--;
		btns.at(NOBtns) = nullptr;

		return true;
	}
	return false;
}

void GUI::DescriptionBox::RemoveAllButtons()
{
	for (int i = 1; i < maxNOBtns; i++)
	{
		btns.at(i) = nullptr;
	}
	NOBtns = 1;
}

void GUI::DescriptionBox::ResetStateBtns()
{
	for (int i = 0; i < NOBtns; i++)
	{
		btns.at(i)->state = btnStates::BTN_IDLE;
	}
}

void GUI::DescriptionBox::ChangeTextureRect(sf::IntRect texRect)
{
	img.setTextureRect(texRect);
}

void GUI::DescriptionBox::ChangeTitle(const std::string& text)
{
	this->title.setString(text);
	this->title.setPosition(img.getGlobalBounds().left + img.getGlobalBounds().width + ((PeToPiX(sizePerc.x, currentRes.x) - (img.getGlobalBounds().width + PeToPiX(sizePerc.x, currentRes.x) * 0.05f)) / 2) - this->title.getGlobalBounds().width, PeToPiY(posPerc.y, currentRes.y) + this->title.getGlobalBounds().height + PeToPiY(sizePerc.y, currentRes.y) * 0.05);
}

void GUI::DescriptionBox::ChangeDescription(const std::string& text)
{
	this->description.setString(text);
	this->description.setString(FitTextInsideBox(this->description, (PeToPiX(sizePerc.x, currentRes.x) / 10) * 8));
	this->description.setPosition(PeToPiX(posPerc.x, currentRes.x) + (PeToPiX(sizePerc.x, currentRes.x) / 20), this->title.getGlobalBounds().top + this->title.getGlobalBounds().height + PeToPiY(sizePerc.y, currentRes.y) * 0.1);
}

void GUI::DescriptionBox::ChangeLeftSpecial(const std::string& text)
{
	leftSpecial.setString(text);
	this->leftSpecial.setPosition(PeToPiX(posPerc.x, currentRes.x) + (PeToPiX(sizePerc.x, currentRes.x) * 0.05), PeToPiY(posPerc.y, currentRes.y) + PeToPiY(sizePerc.y, currentRes.y) - btns.at(0)->GetGlobalBounds().height - (this->leftSpecial.getGlobalBounds().height * 2));
}

void GUI::DescriptionBox::ChangeRightSpecial(const std::string& text)
{
	rightSpecial.setString(text);
	this->rightSpecial.setPosition(PeToPiX(posPerc.x, currentRes.x) + PeToPiX(sizePerc.x, currentRes.x) - this->rightSpecial.getGlobalBounds().width - (PeToPiX(sizePerc.x, currentRes.x) * 0.05), PeToPiY(posPerc.y, currentRes.y) + PeToPiY(sizePerc.y, currentRes.y) - (this->rightSpecial.getGlobalBounds().height * 2) - btns.at(0)->GetGlobalBounds().height);
}

void GUI::DescriptionBox::SetPosition(sf::Vector2f position)
{

	this->posPerc = { PiToPeX(position.x, currentRes.x), PiToPeY(position.y, currentRes.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, currentRes.x), PeToPiY(sizePerc.y, currentRes.y) };

	shape.setPosition(position);

	img.setPosition(sf::Vector2f(position.x + size.x * 0.05f, position.y + size.y * 0.05f));

	this->title.setPosition(img.getGlobalBounds().left + img.getGlobalBounds().width + ((size.x - (img.getGlobalBounds().width + size.x * 0.05f)) / 2) - this->title.getGlobalBounds().width, position.y + this->title.getGlobalBounds().height + size.y * 0.05);

	this->description.setString(FitTextInsideBox(this->description, (size.x / 10) * 8));
	//x - size needs to be divided by 2x the amount that the boundary is
	this->description.setPosition(position.x + (size.x / 20), this->title.getGlobalBounds().top + this->title.getGlobalBounds().height + size.y * 0.1);

	this->leftSpecial.setPosition(position.x + size.x - this->leftSpecial.getGlobalBounds().width - size.x * 0.05, position.y + size.y - this->rightSpecial.getGlobalBounds().width);

	this->rightSpecial.setPosition(position.x + size.x * 0.05, position.y + size.y - this->rightSpecial.getGlobalBounds().width);

	for (int i = 0; i < 5; i++)
	{
		if (btns.at(i) != nullptr)
		{
			btns.at(i)->AferInit(currentRes);
			btns.at(i)->SetPosition(position.x + btns.at(0)->GetGlobalBounds().width * i, position.y + size.y - btns.at(0)->GetGlobalBounds().height);
		}
	}
}

void GUI::DescriptionBox::SetSize(sf::Vector2f size)
{
	sf::Vector2f position = { PeToPiX(posPerc.x, currentRes.x), PeToPiY(posPerc.y, currentRes.y) };
	this->sizePerc = { PiToPeX(size.x, currentRes.x), PiToPeY(size.y, currentRes.y) };

	shape.setSize(size);

	img.setSize(sf::Vector2f(size.y / 6, size.y / 6)); //all 	y or x so its uniform
	img.setPosition(sf::Vector2f(position.x + size.x * 0.05f, position.y + size.y * 0.05f));

	this->title.setPosition(img.getGlobalBounds().left + img.getGlobalBounds().width + ((size.x - (img.getGlobalBounds().width + size.x * 0.05f)) / 2) - this->title.getGlobalBounds().width, position.y + this->title.getGlobalBounds().height + size.y * 0.05);

	this->description.setString(FitTextInsideBox(this->description, (size.x / 10) * 8));
	//x - size needs to be divided by 2x the amount that the boundary is
	this->description.setPosition(position.x + (size.x / 20), this->title.getGlobalBounds().top + this->title.getGlobalBounds().height + size.y * 0.1);

	this->leftSpecial.setPosition(position.x + size.x - this->leftSpecial.getGlobalBounds().width - size.x * 0.05, position.y + size.y - this->rightSpecial.getGlobalBounds().width);

	this->rightSpecial.setPosition(position.x + size.x * 0.05, position.y + size.y - this->rightSpecial.getGlobalBounds().width);

	for (int i = 0; i < 5; i++)
	{
		if (btns.at(i) != nullptr)
		{
			btns.at(i)->AferInit(currentRes);
			btns.at(i)->SetPosition(position.x + btns.at(0)->GetGlobalBounds().width * i, position.y + size.y - btns.at(0)->GetGlobalBounds().height);
		}
	}
}

void GUI::DescriptionBox::LoadItem(ItemData& data)
{
	this->data = data;

	ChangeTextureRect(sf::IntRect(data.texPosition.x, data.texPosition.y, 32, 32));
	ChangeTitle(data.name);
	ChangeDescription(data.description);
	ChangeLeftSpecial(std::string("Amount:" + std::to_string(data.count)));
	if (data.equiped == true)
	{
		ChangeRightSpecial(std::string("Equiped"));
	}
	else
	{
		ChangeRightSpecial(std::string(""));
	}
}

/**********************Pubblic functions**********************/
void GUI::DescriptionBox::AfterInit(sf::Vector2f res)
{
	this->currentRes = res;
	sf::Vector2f position = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };

	shape.setSize(size);
	shape.setPosition(position);

	img.setSize(sf::Vector2f(size.y / 6, size.y / 6)); //all 	y or x so its uniform
	img.setPosition(sf::Vector2f(position.x + size.x * 0.05f, position.y + size.y * 0.05f));

	this->title.setCharacterSize(CaclCharSize(res));
	this->description.setCharacterSize(CaclCharSizeSmall(res));
	this->leftSpecial.setCharacterSize(CaclCharSizeSmall(res));
	this->rightSpecial.setCharacterSize(CaclCharSizeSmall(res));

	this->title.setPosition(img.getGlobalBounds().left + img.getGlobalBounds().width + ((size.x - (img.getGlobalBounds().width + size.x * 0.05f)) / 2) - this->title.getGlobalBounds().width, position.y + this->title.getGlobalBounds().height + size.y * 0.05);

	this->description.setString(FitTextInsideBox(this->description, (size.x / 10) * 8));
	//x - size needs to be divided by 2x the amount that the boundary is
	this->description.setPosition(position.x + (size.x / 20), this->title.getGlobalBounds().top + this->title.getGlobalBounds().height + size.y * 0.1);

	for (int i = 0; i < 5; i++)
	{
		if (btns.at(i) != nullptr)
		{
			btns.at(i)->AferInit(res);
			btns.at(i)->SetPosition(position.x + btns.at(0)->GetGlobalBounds().width * i, position.y + size.y - btns.at(0)->GetGlobalBounds().height);
		}
	}

	this->leftSpecial.setPosition(position.x + (size.x * 0.05), position.y + size.y - btns.at(0)->GetGlobalBounds().height - (this->leftSpecial.getGlobalBounds().height * 2));

	this->rightSpecial.setPosition(position.x + size.x - this->rightSpecial.getGlobalBounds().width - (size.x * 0.05), position.y + size.y - (this->rightSpecial.getGlobalBounds().height * 2) - btns.at(0)->GetGlobalBounds().height);
}
void GUI::DescriptionBox::Update(sf::Vector2i mousePosWindow, float dt)
{
	UpdateKeyTimer(dt);
	if (shown == true)
	{
		for (std::unique_ptr<GUI::Button>& a : btns)
		{
			if (a != nullptr)
			{
				a->Update(mousePosWindow);
			}
		}
	}
}
void GUI::DescriptionBox::Display(sf::RenderTarget& trg)
{
	if (shown)
	{
		trg.draw(shape);
		trg.draw(img);
		trg.draw(title);
		trg.draw(description);
		trg.draw(leftSpecial);
		trg.draw(rightSpecial);
		for (std::unique_ptr<GUI::Button>& a : btns)
		{
			if (a != nullptr)
			{
				a->Display(trg);
			}
		}
	}
}

/**********************Private functions**********************/
void GUI::DescriptionBox::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}
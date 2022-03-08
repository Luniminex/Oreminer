#include "GUI/GUI.hpp"

/*****************************Constructors / destructors********************************/

GUI::Displaybox::Displaybox(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
	sf::Texture* texToDisplay, ItemData& data, sf::Font& font, sf::Vector2f res) :
	TexturedGUI()
{

	this->item = data;
	textureType = btnTypes::big;
	textured = true;
	this->currentRes = res;

	this->posPerc = { posXPerc, posYPerc };
	this->sizePerc = sizePerc;

	float posX = PeToPiX(posPerc.x, res.x);
	float posY = PeToPiY(posPerc.y, res.y);

	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiX(sizePerc.y, res.y) };

	sf::Vector2f imgSize = { size.y, size.y };

	img.setTexture(texToDisplay);
	img.setTextureRect(sf::IntRect(item.texPosition.x, item.texPosition.y, 32, 32));
	img.setSize(imgSize);
	img.setPosition(posX, posY);

	shape.setSize(sf::Vector2f(size.x, imgSize.y));
	shape.setPosition(posX, posY);
	shape.setTexture(&btnTextures.at(textureType));
	sf::IntRect tempRect;
	tempRect.left = 0;
	tempRect.top = 0;
	tempRect.height = btnTextures.at(textureType).getSize().y / 3;
	tempRect.width = btnTextures.at(textureType).getSize().x;
	shape.setTextureRect(tempRect);

	name.setFont(font);
	name.setCharacterSize(CaclCharSizeSmall(res));
	name.setString(data.name);
	name.setPosition(img.getPosition().x + img.getSize().x + ((shape.getSize().x - img.getSize().x) / 2.f), img.getPosition().y);
	name.setFillColor(sf::Color(224, 221, 175));

	std::string temp = "Amount:" + std::to_string(data.count);

	detailsLeft.setFont(font);
	detailsLeft.setCharacterSize(CaclCharSizeSmallest(res));
	detailsLeft.setString(temp);
	detailsLeft.setPosition(img.getPosition().x + img.getSize().x, img.getPosition().y + img.getSize().y - detailsLeft.getGlobalBounds().height * 2);
	detailsLeft.setFillColor(sf::Color(224, 221, 175));

	if (data.equiped == true)
	{
		temp = "Equiped";
	}
	else
	{
		temp = "";
	}
	detailsRight.setFont(font);
	detailsRight.setCharacterSize(CaclCharSizeSmallest(res));
	detailsRight.setString(temp);
	detailsRight.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width - detailsRight.getGlobalBounds().width, img.getPosition().y + img.getSize().y - detailsRight.getGlobalBounds().height * 2);
	detailsRight.setFillColor(sf::Color(224, 221, 175));
}

GUI::Displaybox::~Displaybox()
{
}

/*****************************accessors**************************************/

unsigned int GUI::Displaybox::GetState()
{
	return state;
}

sf::Vector2f GUI::Displaybox::GetSize()
{
	return this->shape.getSize();
}

sf::Vector2f GUI::Displaybox::GetPosition()
{
	return this->shape.getPosition();
}

ItemData& GUI::Displaybox::GetItemData()
{
	return item;
}

/*****************************modifiers**************************************/

void GUI::Displaybox::SetPosition(float x, float y)
{
	img.setPosition(x, y);

	shape.setPosition(x, y);

	name.setPosition(img.getPosition().x + img.getSize().x + ((shape.getSize().x - img.getSize().x) / 2.f), img.getPosition().y);

	detailsLeft.setPosition(img.getPosition().x + img.getSize().x, img.getPosition().y + img.getSize().y - detailsLeft.getGlobalBounds().height * 2);

	detailsRight.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width - detailsRight.getGlobalBounds().width, img.getPosition().y + img.getSize().y - detailsRight.getGlobalBounds().height * 2);

	//shape.setPosition(x - padding/2.f,y - padding /2.f);
}

void GUI::Displaybox::SetPercPosition(float xPerc, float yPerc)
{
	float x = PeToPiX(xPerc, currentRes.x);
	float y = PeToPiY(yPerc, currentRes.y);

	shape.setPosition(x, y);

	img.setPosition(x, y);

	name.setCharacterSize(CaclCharSizeSmall(currentRes));
	name.setPosition(img.getPosition().x + img.getSize().x + ((shape.getSize().x - img.getSize().x) / 2.f), img.getPosition().y);

	detailsLeft.setCharacterSize(CaclCharSizeSmallest(currentRes));
	detailsLeft.setPosition(img.getPosition().x + img.getSize().x, img.getPosition().y + img.getSize().y - detailsLeft.getGlobalBounds().height * 2);

	detailsRight.setCharacterSize(CaclCharSizeSmall(currentRes));
	detailsRight.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width - detailsRight.getGlobalBounds().width, img.getPosition().y + img.getSize().y - detailsRight.getGlobalBounds().height * 2);
}

void GUI::Displaybox::ChangeTitle(const std::string& txt)
{
	name.setString(txt);
	name.setPosition(img.getPosition().x + img.getSize().x + ((shape.getSize().x - img.getSize().x) / 2.f), img.getPosition().y);
}

void GUI::Displaybox::ChangeLeftSpecial(const std::string& txt)
{
	detailsLeft.setString(txt);
	detailsLeft.setPosition(img.getPosition().x + img.getSize().x, img.getPosition().y + img.getSize().y - detailsLeft.getGlobalBounds().height * 2);
}

void GUI::Displaybox::ChangeRightSpecial(const std::string& txt)
{
	detailsRight.setString(txt);
	detailsRight.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width - detailsRight.getGlobalBounds().width, img.getPosition().y + img.getSize().y - detailsRight.getGlobalBounds().height * 1.2);
}
/*****************************public fuctions**************************************/

void GUI::Displaybox::AfterInit(sf::Vector2f res)
{
	currentRes = res;

	float x = PeToPiX(posPerc.x, res.x);
	float y = PeToPiY(posPerc.y, res.y);

	sf::Vector2f size = { PeToPiX(this->sizePerc.x, res.x), PeToPiY(this->sizePerc.y, res.y) };

	sf::Vector2f imgSize = { size.y, size.y };

	shape.setSize(sf::Vector2f(size.x, imgSize.y));
	shape.setPosition(x, y);

	img.setSize(imgSize);
	img.setPosition(x, y);

	name.setCharacterSize(CaclCharSizeSmall(res));
	name.setPosition(img.getPosition().x + img.getSize().x + ((shape.getSize().x - img.getSize().x) / 2.f), img.getPosition().y);

	detailsLeft.setCharacterSize(CaclCharSizeSmallest(res));
	detailsLeft.setPosition(img.getPosition().x + img.getSize().x, img.getPosition().y + img.getSize().y - detailsLeft.getGlobalBounds().height * 2);

	detailsRight.setCharacterSize(CaclCharSizeSmall(res));
	detailsRight.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width - detailsRight.getGlobalBounds().width, img.getPosition().y + img.getSize().y - detailsRight.getGlobalBounds().height * 2);
}

void GUI::Displaybox::Update(sf::Vector2i mousePosWin)
{
	state = btnStates::BTN_IDLE;

	if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWin)))
	{
		state = btnStates::BTN_HOVER;
		sf::IntRect tempRect;
		tempRect.left = 0;
		tempRect.top = btnTextures.at(textureType).getSize().y / 3;
		tempRect.height = btnTextures.at(textureType).getSize().y / 3;
		tempRect.width = btnTextures.at(textureType).getSize().x;
		shape.setTextureRect(tempRect);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			state = btnStates::BTN_PRESSED;
			sf::IntRect tempRect;
			tempRect.left = 0;
			tempRect.top = (btnTextures.at(textureType).getSize().y / 3) * 2;
			tempRect.height = btnTextures.at(textureType).getSize().y / 3;
			tempRect.width = btnTextures.at(textureType).getSize().x;
			shape.setTextureRect(tempRect);
			AudioPlayer::PlaySound("BTNCLICK");
		}
	}
	else
	{
		sf::IntRect tempRect;
		tempRect.left = 0;
		tempRect.top = 0;
		tempRect.height = btnTextures.at(textureType).getSize().y / 3;
		tempRect.width = btnTextures.at(textureType).getSize().x;
		shape.setTextureRect(tempRect);
	}
}

void GUI::Displaybox::Display(sf::RenderTarget& trg)
{
	trg.draw(shape);
	trg.draw(img);
	trg.draw(name);
	trg.draw(detailsLeft);
	trg.draw(detailsRight);
}
#include "GUI/GUI.hpp"

GUI::CraftBox::CraftBox(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Texture& tex, std::vector<sf::Vector2f> texCoords, std::vector<int> NOitemsNeeded, std::vector<int> ingredientsID, int resultID, sf::Font& font, sf::Vector2f res) :
	TexturedGUI()
{
	this->posPerc = posPerc;
	this->sizePerc = sizePerc;
	this->res = res;
	this->shown = true;
	this->ingredientsID = ingredientsID;
	this->amounts = NOitemsNeeded;
	this->resultID = resultID;
	this->state = btnStates::BTN_IDLE;
	textureType = btnTypes::big;
	textured = true;

	box.setTexture(&btnTextures.at(textureType));
	sf::IntRect temp;
	temp.left = 0;
	temp.top = 0;
	temp.height = btnTextures.at(textureType).getSize().y / 3;
	temp.width = btnTextures.at(textureType).getSize().x;
	box.setTextureRect(temp);

	sf::Vector2f pos = { PeToPiX(posPerc.x, res.x), PeToPiY(posPerc.y, res.y) };
	sf::Vector2f size = { PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) };
	unsigned int fontSize = CaclCharSizeSmallest(res);

	box.setPosition(pos);
	box.setSize(size);

	int currentPos = 0;
	for (auto& a : ingredientsID)
	{
		(void)a;
		ingredients.push_back(sf::RectangleShape(sf::Vector2f(size.y, size.y)));
		ingredients.at(currentPos).setPosition(sf::Vector2f(pos.x + (size.y * currentPos), pos.y));
		ingredients.at(currentPos).setTexture(&tex);
		ingredients.at(currentPos).setTextureRect(sf::IntRect(sf::Vector2i(texCoords.at(currentPos).x, texCoords.at(currentPos).y), sf::Vector2i(32, 32)));

		NOIngredients.push_back(sf::Text());
		NOIngredients.at(currentPos).setFont(font);
		NOIngredients.at(currentPos).setCharacterSize(fontSize);
		NOIngredients.at(currentPos).setString(std::to_string(NOitemsNeeded.at(currentPos)));
		NOIngredients.at(currentPos).setPosition(sf::Vector2f((size.y * currentPos) + pos.x + size.y - NOIngredients.at(currentPos).getGlobalBounds().width, pos.y + size.y - NOIngredients.at(currentPos).getGlobalBounds().height));
		currentPos++;
	}

	result.setSize(sf::Vector2f(size.y, size.y));
	result.setTexture(&tex);
	result.setTextureRect(sf::IntRect(sf::Vector2i(texCoords.back().x, texCoords.back().y), sf::Vector2i(32, 32)));
	result.setPosition(pos.x + size.x - size.y, pos.y);
}

GUI::CraftBox::~CraftBox()
{
}

int GUI::CraftBox::GetState()
{
	return this->state;
}

bool GUI::CraftBox::IsShown()
{
	return shown;
}

int GUI::CraftBox::GetResultID()
{
	return resultID;
}

std::vector<int> GUI::CraftBox::GetIngredientsID()
{
	return ingredientsID;
}

std::vector<int> GUI::CraftBox::GetAmounts()
{
	return amounts;
}

void GUI::CraftBox::SetShown(bool shown)
{
	this->shown = shown;
}

void GUI::CraftBox::FunkyStuff()
{
	idleClr = sf::Color(25, 25, 25);
}

void GUI::CraftBox::Update(sf::Vector2i mousePosWindow)
{
	if (shown)
	{
		state = btnStates::BTN_IDLE;

		if (box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			state = btnStates::BTN_HOVER;
			sf::IntRect temp;
			temp.left = 0;
			temp.top = btnTextures.at(textureType).getSize().y / 3;
			temp.height = btnTextures.at(textureType).getSize().y / 3;
			temp.width = btnTextures.at(textureType).getSize().x;
			box.setTextureRect(temp);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				state = btnStates::BTN_PRESSED;
				sf::IntRect temp;
				temp.left = 0;
				temp.top = (btnTextures.at(textureType).getSize().y / 3) * 2;
				temp.height = btnTextures.at(textureType).getSize().y / 3;
				temp.width = btnTextures.at(textureType).getSize().x;
				box.setTextureRect(temp);
			}
		}
		else
		{
			sf::IntRect temp;
			temp.left = 0;
			temp.top = 0;
			temp.height = btnTextures.at(textureType).getSize().y / 3;
			temp.width = btnTextures.at(textureType).getSize().x;
			box.setTextureRect(temp);
		}
	}
}

void GUI::CraftBox::Display(sf::RenderTarget& trg)
{
	trg.draw(box);

	for (auto& a : ingredients)
	{
		trg.draw(a);
	}
	for (auto& a : NOIngredients)
	{
		trg.draw(a);
	}

	trg.draw(result);
}
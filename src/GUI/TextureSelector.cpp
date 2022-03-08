#include "GUI/GUI.hpp"

//constructors / destructors
GUI::TextureSelector::TextureSelector(float xPerc, float yPerc, sf::Vector2f btnSizePerc, const std::string& btnText,
	float texWidth, float texHeight, float gridSize, sf::Vector2f tileSize, sf::Font& font, sf::Texture& texture, sf::Vector2f res) :
	texSheet(texture),
	hideButton(xPerc, yPerc, btnSizePerc, font, btnText, GUI::btnTypes::basic, res)
{

	this->posPerc = { xPerc, yPerc };
	this->sizePerc = btnSizePerc;

	float x = PeToPiX(xPerc, res.x);
	float y = PeToPiY(yPerc, res.y);

	bounds.setPosition(x + hideButton.GetSize().x, y);
	bounds.setFillColor(sf::Color(91, 92, 90, 200));
	bounds.setOutlineColor(sf::Color(255, 255, 255));
	bounds.setOutlineThickness(3.f);

	texSheet.setPosition(x + hideButton.GetSize().x, y);
	texSheet.setTexture(texture);

	selectedRect.width = tileSize.x;
	selectedRect.height = tileSize.y;
	selectedRect.left = 0;
	selectedRect.top = 0;

	sf::IntRect temp;
	temp.left = 0;
	temp.top = 0;
	temp.width = texWidth;
	temp.height = texHeight;

	texSheet.setTextureRect(temp);

	hidden = true;
	active = false;
	changed = true;

	gridSize = gridSize;
	this->tileSize = tileSize;

	keyTime = 0.f;
	keyTimeMax = 10.f;

	bounds.setSize(sf::Vector2f(texSheet.getGlobalBounds().width, texSheet.getGlobalBounds().height));

	gridPosInside = { 0, 0 };
}

/*************************modifiers*******************************/
void GUI::TextureSelector::SetPosition(float x, float y)
{
	this->hideButton.SetPosition(x, y);
	bounds.setPosition(x + hideButton.GetSize().x, y);
	texSheet.setPosition(x + hideButton.GetSize().x, y);
}

void GUI::TextureSelector::SetSize(float x, float y)
{
	this->hideButton.SetSize(x, y);
}

void GUI::TextureSelector::SetCharSize(unsigned int size)
{
	this->hideButton.SetCharSize(size);
}

void GUI::TextureSelector::setTex(sf::Texture& tex)
{
	texSheet.setTexture(tex);
}

GUI::TextureSelector::~TextureSelector()
{
}

/*******************************accessors****************************/

bool GUI::TextureSelector::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

bool GUI::TextureSelector::WasChanged()
{
	if (changed == true)
	{ //if was changed return true and set it to false
		changed = false;
		std::cout << "Returned true" << std::endl;
		return true;
	}
	return changed;
}

sf::IntRect& GUI::TextureSelector::GetSelectedTexture()
{
	return selectedRect;
}

int GUI::TextureSelector::GetSelectedID()
{
	return ((selectedRect.top / 32) + ((selectedRect.left / 32) * ((texSheet.getGlobalBounds().height + 32) / 32)));
}

sf::Vector2f GUI::TextureSelector::GetBtnSize()
{
	return hideButton.GetSize();
}

/**************************public functions***************************/

void GUI::TextureSelector::AferInit(sf::Vector2f res)
{
	hideButton.AferInit(res);
	bounds.setPosition(PeToPiX(posPerc.x, res.x) + hideButton.GetSize().x, PeToPiY(posPerc.y, res.y));
	texSheet.setPosition(PeToPiX(posPerc.x, res.x) + hideButton.GetSize().x, PeToPiY(posPerc.y, res.y));
}

void GUI::TextureSelector::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

void GUI::TextureSelector::Update(float dt, sf::Vector2i mousePosWindow)
{
	UpdateKeyTimer(dt);
	hideButton.Update(mousePosWindow);

	if (hideButton.GetState() == btnStates::BTN_PRESSED && GetKeyTimer() == true)
	{
		hidden = !hidden;
	}

	if (!hidden)
	{
		active = false;
		if (bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) == true)
		{
			hideButton.state = btnStates::BTN_HOVER; // sets state to hover when hovering over opened texture selector
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true && GetKeyTimer() == true)
			{
				active = true;
				gridPosInside.x = (mousePosWindow.x - static_cast<int>(bounds.getPosition().x)) / static_cast<int>(tileSize.x);
				gridPosInside.y = (mousePosWindow.y - static_cast<int>(bounds.getPosition().y)) / static_cast<int>(tileSize.y);

				selectedRect.top = gridPosInside.y * tileSize.y;
				selectedRect.left = gridPosInside.x * tileSize.x;

				changed = true;
			}
		}
	}
}

void GUI::TextureSelector::Display(sf::RenderTarget& trg)
{

	hideButton.Display(trg);

	if (hidden == false)
	{
		trg.draw(bounds);
		trg.draw(texSheet);
	}
}
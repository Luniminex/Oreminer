#include "GUI/GUI.hpp"

float GUI::PeToPiX(const float perc, float resWidth)
{
	return std::floor(resWidth * (perc / 100.f));
}

float GUI::PeToPiY(const float perc, float resHeight)
{
	return std::floor(resHeight * (perc / 100.f));
}

float GUI::PiToPeX(const float pixelValue, float resWidth)
{
	return (pixelValue / resWidth) * 100;
}
float GUI::PiToPeY(const float pixelValue, float resHeight)
{
	return (pixelValue / resHeight) * 100;
}

/****************************/

unsigned int GUI::CaclCharSize(sf::Vector2f res)
{
	return static_cast<unsigned>((res.x + res.y) / 120);
}

unsigned int GUI::CaclCharSizeSmall(sf::Vector2f res)
{
	return static_cast<unsigned>((res.x + res.y) / 180);
}

unsigned int GUI::CaclCharSizeSmallest(sf::Vector2f res)
{
	return static_cast<unsigned>((res.x + res.y) / 210);
}

/********************************************************/

unsigned int GUI::CaclCharSize(float resWidth, float resHeight)
{
	return static_cast<unsigned>((resWidth + resHeight) / 120);
}

unsigned int GUI::CaclCharSizeSmall(float resWidth, float resHeight)
{
	return static_cast<unsigned>((resWidth + resHeight) / 180);
}

unsigned int GUI::CaclCharSizeSmallest(float resWidth, float resHeight)
{
	return static_cast<unsigned>((resWidth + resHeight) / 210);
}

std::string GUI::FitTextInsideBox(sf::Text& txt, float bound)
{
	std::string str = txt.getString();
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	txt.setString(str);
	int lenght = str.length();
	float strSize = txt.getGlobalBounds().width;

	float howManyTimesBigger = strSize / bound;
	bool isTxtSmaller = false;

	//if text bigger than bound
	if (howManyTimesBigger > 1)
	{
		isTxtSmaller = false;
	}
	else
	{
		isTxtSmaller = true;
	}
	if (!isTxtSmaller)
	{
		howManyTimesBigger = std::ceil(howManyTimesBigger);
		float offset = lenght / howManyTimesBigger; // if for example 2x bigger and size of txt is 100, insert new line at 50

		float currentOffset;
		bool whiteSpace = false;
		for (int i = 1; i < howManyTimesBigger; i++)
		{
			whiteSpace = false;
			currentOffset = offset * i;
			while (whiteSpace == false)
			{
				if (str.at(currentOffset) == ' ')
				{
					whiteSpace = true;
				}
				else
				{
					whiteSpace = false;
					currentOffset--;
					if (currentOffset < 0)
					{
						currentOffset = offset * i;
						whiteSpace = true;
					}
				}
			}
			str.insert(str.begin() + currentOffset, '\n');
		}
		return str;
	}
	else
	{
		return str;
	}
}

std::map<int, sf::Texture> GUI::TexturedGUI::btnTextures;

GUI::TexturedGUI::TexturedGUI()
{
}
GUI::TexturedGUI::~TexturedGUI()
{
}

void GUI::TexturedGUI::LoadTextures()
{
	ResourceManager* resourceman = ResourceManager::GetInstance();

	btnTextures[GUI::btnTypes::basic] = sf::Texture();
	if (!btnTextures.at(GUI::btnTypes::basic).loadFromFile(resourceman->TextureAt("BASICBTN")))
	{
		std::cout << "GUI::BUTTON:: COULD NOT LOAD BTN TEXTURE" << std::endl;
	}
	btnTextures[GUI::btnTypes::big] = sf::Texture();
	if (!btnTextures.at(GUI::btnTypes::big).loadFromFile(resourceman->TextureAt("BIGBTN")))
	{
		std::cout << "GUI::BUTTON:: COULD NOT LOAD BTN TEXTURE" << std::endl;
	}
	btnTextures[GUI::btnTypes::bgr] = sf::Texture();
	if (!btnTextures.at(GUI::btnTypes::bgr).loadFromFile(resourceman->TextureAt("BGRBTN")))
	{
		std::cout << "GUI::BUTTON:: COULD NOT LOAD BTN TEXTURE" << std::endl;
	}
	btnTextures[GUI::btnTypes::wide] = sf::Texture();
	if (!btnTextures.at(GUI::btnTypes::wide).loadFromFile(resourceman->TextureAt("WIDEBTN")))
	{
		std::cout << "GUI::BUTTON:: COULD NOT LOAD BTN TEXTURE" << std::endl;
	}
}

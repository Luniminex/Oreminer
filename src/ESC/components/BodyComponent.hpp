#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include <fstream>
#include <iostream>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>

//component that contains sprite, other components required to interact
class BodyComponent
{
	//variables
private:

	sf::Sprite sprite;
	sf::Texture& tex;

	//functions
public:
	BodyComponent(sf::Texture& tex, sf::Vector2f scale);

	sf::Sprite& GetSprite();

	sf::Texture& GetTexture();

	sf::Vector2f GetPos();

	void SetPos(sf::Vector2f pos);

	void Display(sf::RenderTarget& target);

};
#endif
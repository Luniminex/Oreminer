#ifndef MININGCOMPONENT_HPP
#define MININGCOMPONENT_HPP

class MiningComponent
{

	//vars
public:
	sf::Vector2f offset; //offset from side
	bool mining;
	bool mined;
	//constructor
public:
	MiningComponent(sf::Vector2f offset);
	~MiningComponent();

public:
	//returns point on right side of box
	sf::Vector2f MineRight(sf::FloatRect hitboxRect);

	//returns point on left side of box
	sf::Vector2f MineLeft(sf::FloatRect hitboxRect);
};

#endif
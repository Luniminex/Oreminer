#ifndef PORTALDATA_HPP
#define PORTALDATA_HPP

class PortalData
{

	//variables
public:
	int id;
	sf::Vector2f texCoords;
	std::string mapPath;
	//connstructor / destructor
public:
	PortalData(int id, sf::Vector2f texCoords, const std::string& mapPath);
	~PortalData();
};

#endif
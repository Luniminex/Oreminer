#include "containers/PortalData.hpp"

PortalData::PortalData(int id, sf::Vector2f texCoords, const std::string& mapPath)
{
	this->id = id;
	this->texCoords = texCoords;
	this->mapPath = mapPath;
}

PortalData::~PortalData()
{
}

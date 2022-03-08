#include "containers/GraphicsContainer.hpp"

//constructor / destructor
GraphicsContainer::GraphicsContainer()
{
}

GraphicsContainer::GraphicsContainer(const std::string title, unsigned short frameLimit,
	bool vsync, bool fullscreen, int width, int height, int bitsPerPixel)
{
	this->windowTitle = title;
	this->frameLimit = frameLimit;
	this->vsync = vsync;
	this->fullscreen = fullscreen;
	this->width = width;
	this->height = height;
	this->bitsPerPixel = bitsPerPixel;
}

GraphicsContainer::GraphicsContainer(const GraphicsContainer& obj)
{
	this->windowTitle = obj.windowTitle;
	this->frameLimit = obj.frameLimit;
	this->vsync = obj.vsync;
	this->width = obj.width;
	this->height = obj.height;
	this->bitsPerPixel = obj.bitsPerPixel;
}

GraphicsContainer::~GraphicsContainer()
{
}

//public functions

void GraphicsContainer::SetWindowTitle(const std::string& str)
{
	this->windowTitle = str;
}

void GraphicsContainer::SetFrameLimit(unsigned short frameLimit)
{
	this->frameLimit = frameLimit;
}

void GraphicsContainer::SetVSync(bool enable)
{
	this->vsync = enable;
}

void GraphicsContainer::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void GraphicsContainer::SetPixelDepth(unsigned short bitsPerPixel)
{
	bitsPerPixel = bitsPerPixel;
}

//accessors

const std::string& GraphicsContainer::GetTitle()
{
	return this->windowTitle;
}

float GraphicsContainer::GetFrameLimit()
{
	return this->frameLimit;
}

bool GraphicsContainer::GetVSync()
{
	return this->vsync;
}

sf::Vector2f GraphicsContainer::GetSize()
{
	return sf::Vector2f(width, height);
}

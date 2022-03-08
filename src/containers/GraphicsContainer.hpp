#ifndef GRAPHICSCONTAINER_HPP
#define GRAPHICSCONTAINER_HPP

#include <string>


//class used for storing settings data
class GraphicsContainer
{
	//variables
public:
	std::string windowTitle;   //name of the window
	unsigned short frameLimit; //frameLimit of the game
	bool vsync;				   //enables disables vsync
	bool fullscreen;		   //fullscreen
	int width;				   //width of window
	int height;				   //height of window
	int bitsPerPixel;		   //bits per pixel depth

	//Constructors
public:
	GraphicsContainer();

	GraphicsContainer(const std::string title, unsigned short frameLimit,
		bool vsync, bool fullscreen, int width, int height, int bitsPerPixel);

	GraphicsContainer(const GraphicsContainer& obj);

	~GraphicsContainer();

	//functions
public:
	//sets the title of the window
	void SetWindowTitle(const std::string& str);

	//sets frameLimit
	void SetFrameLimit(unsigned short frameLimit);

	//enables/disables vsync
	void SetVSync(bool enable);

	//sets width and height of window
	void SetSize(int width, int height);

	//sets pixel depths in bits per pixel
	void SetPixelDepth(unsigned short bitsPerPixel);

	//accessors
public:
	//returns title of the window
	const std::string& GetTitle();

	//returns frame Limit
	float GetFrameLimit();

	//returns true if vsync is set
	bool GetVSync();

	//returns size of the window
	sf::Vector2f GetSize();
};

#endif
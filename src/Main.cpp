#include "Platform/Platform.hpp"
#include "Platform/Win32/WindowsPlatform.hpp"
#include "game/Game.hpp"

int main()
{
	util::Platform platform;
#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	Game game = Game();
	game.Run();

	return 0;
}

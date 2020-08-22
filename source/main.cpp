#include <platypus.h>

int main()
{
	auto game = std::make_shared<Platypus>("demo game");
	game->initialize();
	auto ret = game->run();
	game->shutdown();

	return ret;
}

#ifdef _WIN32
	#include <Windows.h>

	int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
	{
		return main();
	}
#endif

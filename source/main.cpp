
int main()
{
	return 0;
}

#ifdef _WIN32
	#include <Windows.h>

	int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
	{
		return main();
	}
#endif

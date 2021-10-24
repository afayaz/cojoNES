#include "System.hpp"

int main(int argc, char** argv)
{
	//FILE* rom = fopen("test.nes", "rb");
	//fclose(rom);

	System system;

	bool loop = true;
	while (loop)
	{
		loop = system.Process();
	}

	return 0;
}

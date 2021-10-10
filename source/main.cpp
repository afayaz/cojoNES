#include "CPU.hpp"

int main(int argc, char** argv)
{
	//FILE* rom = fopen("test.nes", "rb");
	//fclose(rom);

	CPU cpu;

	bool loop = true;
	while (loop)
	{
		loop = cpu.Process();
	}

	return 0;
}

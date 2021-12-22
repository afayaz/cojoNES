#include <memory>

#include "CPU.hpp"
#include "Memory.hpp"
#include "System.hpp"

int main(int argc, char** argv)
{
	//FILE* rom = fopen("test.nes", "rb");
	//fclose(rom);

	std::shared_ptr<CPU>    cpu     = std::make_shared<CPU>();
	std::shared_ptr<Memory> memory  = std::make_shared<Memory>();
	std::shared_ptr<System> system  = std::make_shared<System>(cpu, memory);

	cpu->ConnectSystem(system);

	bool loop = true;
	while (loop)
	{
		loop = system->Process();
	}

	return 0;
}

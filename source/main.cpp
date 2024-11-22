#include <memory>

#include <spdlog/spdlog.h>

#include "CPU.hpp"
#include "Memory.hpp"
#include "System.hpp"
#include "Cartridge.hpp"

int main(int argc, char** argv)
{
	spdlog::set_pattern("[%Y/%m/%d %T.%e] [%s:%#] [%l] %v");

	if (argc < 2)
	{
		SPDLOG_ERROR("No ROM file specified!");
		return 1;
	}

	std::shared_ptr<CPU>    cpu     = std::make_shared<CPU>();
	std::shared_ptr<Memory> memory  = std::make_shared<Memory>();
	std::shared_ptr<Cartridge> cart = std::make_shared<Cartridge>();

	bool isRomValid = cart->Load(argv[1]);

	if (!isRomValid)
	{
		SPDLOG_ERROR("File \"{}\" is not a valid NES ROM", argv[1]);
		return 1;
	}

	// Initialise system now that ROM is loaded.
	std::shared_ptr<System> system  = std::make_shared<System>(cpu, memory, cart);
	system->Reset();

	bool loop = true;
	while (loop)
	{
		loop = system->Process();
	}

	return 0;
}

#include "System.hpp"

#include "CPU.hpp"
#include "Memory.hpp"
#include "Cartridge.hpp"

System::System(std::shared_ptr<CPU> cpu, std::shared_ptr<Memory> memory, std::shared_ptr<Cartridge> cartridge)
	: mCPU(cpu)
	, mMemory(memory)
	, mCartridge(cartridge)
{
}

void System::Reset()
{
	mCPU->Reset();
}

bool System::Process()
{
	return mCPU->Process();
}

uint8_t System::Read(uint16_t address)
{
	if (address >= 0x8000)
	{
		return mCartridge->Read(address);
	}

	// For now, use the entire address space for RAM.
	return mMemory->Read(address);
}

void System::Write(uint16_t address, uint8_t data)
{
	// For now, use the entire address space for RAM.
	mMemory->Write(address, data);
}
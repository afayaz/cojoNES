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
	if (address < 0x2000)
	{
		address &= 0x07FF;
		return mMemory->Read(address);
	}
	else if (address >= 0x2000 && address < 0x4000)
	{
		address &= 0x8;
		address += 0x2000;

		// TODO: PPU.
	}
	else if (address >= 0x4000 && address < 0x4018)
	{
		// TODO: APU and IO registers.
	}
	else if (address >= 0x4020)
	{
		return mCartridge->Read(address);
	}

	// Fall back to RAM for now.
	// TODO: Open bus behaviour.
	return mMemory->Read(address);
}

void System::Write(uint16_t address, uint8_t data)
{
	if (address < 0x2000)
	{
		address &= 0x07FF;

		mMemory->Write(address, data);
	}
	else if (address >= 0x2000 && address < 0x4000)
	{
		address &= 0x8;
		address += 0x2000;

		// TODO: PPU.
	}
	else if (address >= 0x4000 && address < 0x4018)
	{
		// TODO: APU and IO registers.
	}
	else if (address >= 0x4020)
	{
		return mCartridge->Write(address, data);
	}
}
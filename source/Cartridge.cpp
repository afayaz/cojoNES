#include "Cartridge.hpp"

#include "ROM.hpp"

bool Cartridge::Load(const std::string& filename)
{
	mRom = std::make_shared<ROM>();
	bool isRomValid = mRom->Load(filename);

	return isRomValid;
}

bool Cartridge::Load()
{
	mRom = std::make_shared<ROM>();
	return mRom->Load();
}

uint8_t Cartridge::Read(uint16_t address)
{
	uint8_t data = 0x00;

	if (RemapAddress(address))
	{
		data = mRom->GetPrgRom()[address];
	}

	return data;
}

void Cartridge::Write(uint16_t address, uint8_t data)
{
	if (RemapAddress(address))
	{
		mRom->GetPrgRom()[address] = data;
	}
}

bool Cartridge::RemapAddress(uint16_t& address)
{
	bool valid = false;

	if (address >= 0x8000 && address <= 0xFFFF)
	{
		// TODO: This assumes 16KB
		address = address & 0x3FFF;
		valid = true;
	}
	else
	{
		printf("Attempted to access invalid address \"%#02X\" from cartridge!\n", address);
	}

	return valid;
}

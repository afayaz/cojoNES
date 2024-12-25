#pragma once

#include <memory>
#include <string>

#include "ROM.hpp"

class Cartridge
{
public:
	bool    Load(const std::string& filename);
	bool    Load();

	uint8_t Read(uint16_t address);
	void    Write(uint16_t address, uint8_t data);

	NESHeader GetHeader() { return mRom ? mRom->GetHeader() : NESHeader{}; }

private:
	bool    RemapAddress(uint16_t& address);

	std::shared_ptr<ROM> mRom;
};

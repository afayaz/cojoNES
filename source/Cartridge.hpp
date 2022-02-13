#pragma once

#include <memory>
#include <string>

class ROM;

class Cartridge
{
public:
	bool    Load(const std::string& filename);

	uint8_t Read(uint16_t address);
	void    Write(uint16_t address, uint8_t data);

private:
	bool    RemapAddress(uint16_t& address);

	std::shared_ptr<ROM> mRom;
};
#pragma once

#include <cstdint>

class Memory
{
public:
	uint8_t Read(uint16_t address) { return a[address]; }
	void    Write(uint16_t address, uint8_t data) { a[address] = data; }

private:
	uint8_t a[0xFFFF];
};
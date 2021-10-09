#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>

class Memory
{
public:
	uint8_t read(uint16_t address) { printf("read %#02x from address %#02x\n", a[address], address); return a[address]; }
	void    write(uint16_t address, uint8_t data) { printf("write %#02x to address %#02x\n", data, address); a[address] = data; }

private:
	uint8_t a[0xFFFF];
};
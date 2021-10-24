#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>

class Memory
{
public:
	uint8_t Read(uint16_t address) { printf("Read %#02X from address %#02X\n", a[address], address); return a[address]; }
	void    Write(uint16_t address, uint8_t data)
	{
		printf("Write %#02X to address %#02X\n", data, address);
		
		a[address] = data;
		
		printf("Surrounding bytes of memory:\n");

		uint16_t base = address & 0xFFF0;

		printf("%#02X: ", base);

		for (size_t i = base; i < base + 0x0F; ++i)
		{
			printf("%#02X ", a[i]);
		}
		printf("\n");
	}

private:
	uint8_t a[0xFFFF];
};
#pragma once

#include <cstdint>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

class Memory
{
public:
	uint8_t Read(uint16_t address) { spdlog::info("Read {:#04x} from address {:#06x}", a[address], address); return a[address]; }
	void    Write(uint16_t address, uint8_t data)
	{
		spdlog::info("Write {:#04x} to address {:#06x}", data, address);
		
		a[address] = data;
		
		spdlog::info("Surrounding bytes of memory:");

		uint16_t base = address & 0xFFF0;

		std::string debugMemBytes = fmt::format("{:#06x}: ", base);

		for (size_t i = base; i < base + 0x0F; ++i)
		{
			debugMemBytes += fmt::format("{:#04x} ", a[i]);
		}
		spdlog::info(debugMemBytes);
	}

private:
	uint8_t a[0xFFFF];
};
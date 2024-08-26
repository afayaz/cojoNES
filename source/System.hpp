#pragma once

#include <cstdint>
#include <memory>

class CPU;
class Memory;
class Cartridge;

class System : public std::enable_shared_from_this<System>
{
public:
	System(std::shared_ptr<CPU> cpu, std::shared_ptr<Memory> memory, std::shared_ptr<Cartridge> cartridge);

	void Reset();
	bool Process();

	uint8_t Read(uint16_t address);
	void    Write(uint16_t address, uint8_t data);
private:
	std::shared_ptr<CPU>       mCPU;
	std::shared_ptr<Memory>    mMemory;
	std::shared_ptr<Cartridge> mCartridge;
};
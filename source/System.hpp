#pragma once

#include <memory>

class CPU;
class Memory;

class System
{
public:
	System();
	~System();

	void Reset();
	bool Process();

	uint8_t Read(uint16_t address);
	void    Write(uint16_t address, uint8_t data);
private:
	std::unique_ptr<CPU>    mCPU;
	std::unique_ptr<Memory> mMemory;
};
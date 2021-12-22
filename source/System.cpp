#include "System.hpp"

#include "CPU.hpp"
#include "Memory.hpp"

System::System(std::shared_ptr<CPU> cpu, std::shared_ptr<Memory> memory)
	: mCPU(cpu)
	, mMemory(memory)
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
	// For now, use the entire address space for RAM.
	return mMemory->Read(address);
}

void System::Write(uint16_t address, uint8_t data)
{
	// For now, use the entire address space for RAM.
	mMemory->Write(address, data);
}
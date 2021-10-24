#include "System.hpp"

#include "CPU.hpp"
#include "Memory.hpp"

System::System()
{
	mMemory = std::make_unique<Memory>();
	mCPU = std::make_unique<CPU>(this);
}

System::~System()
{
	// This is here for the unique_ptr members which need complete types for destruction.
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
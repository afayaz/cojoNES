#include "CPU.hpp"

#include <cstdio>

CPU::CPU()
{
	Reset();
}

void CPU::Reset()
{
	// TODO: Temp data in memory to get started...
	// This is a programme to multiply 10 by 3.
	// Taken from part 2 of OLCs NES emulator series
	uint16_t write_addr = 0x8000;
	memory.write(write_addr++, 0xA2);
	memory.write(write_addr++, 0x0A);
	memory.write(write_addr++, 0x8E);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0xA2);
	memory.write(write_addr++, 0x03);
	memory.write(write_addr++, 0x8E);
	memory.write(write_addr++, 0x01);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0xAC);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0xA9);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0x18);
	memory.write(write_addr++, 0x6D);
	memory.write(write_addr++, 0x01);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0x88);
	memory.write(write_addr++, 0xD0);
	memory.write(write_addr++, 0xFA);
	memory.write(write_addr++, 0x8D);
	memory.write(write_addr++, 0x02);
	memory.write(write_addr++, 0x00);
	memory.write(write_addr++, 0xEA);
	memory.write(write_addr++, 0xEA);
	memory.write(write_addr++, 0xEA);
	
	memory.write(0xFFFC, 0x00);
	memory.write(0xFFFD, 0x80);

	// Use uint16_t to ensure bit shifts don't wrap.
	uint16_t PC_lo = memory.read(0xFFFC);
	uint16_t PC_hi = memory.read(0xFFFD);

	registers.PC = PC_lo | PC_hi << 8;
}

bool CPU::Process()
{
	bool shouldContinue = true;

	printf("PC is %#02x\n", registers.PC);
	Opcodes opcode = static_cast<Opcodes>(memory.read(registers.PC));
	printf("Executing opcode %#02x\n", static_cast<uint8_t>(opcode));
	auto opcodeIter = opTable.find(opcode);
	if (opcodeIter != opTable.end())
	{
		auto& OpFuncs = opcodeIter->second;
		auto fetchFunc = std::bind(OpFuncs.fetchFunc, this);
		auto opFunc = std::bind(OpFuncs.opFunc, this, std::placeholders::_1);

		uint16_t operand = fetchFunc();
		opFunc(operand);

		++registers.PC;
	}
	else
	{
		printf("Illegal opcode %#02x\n", opcode);
		shouldContinue = false;
	}

	return shouldContinue;
}

uint16_t CPU::fetch_immediate()
{
	printf("%s\n", __func__);
	return memory.read(++registers.PC);
}

uint16_t CPU::fetch_absolute()
{
	printf("%s\n", __func__);
	uint16_t lo = memory.read(++registers.PC);
	uint16_t hi = memory.read(++registers.PC);

	return lo | hi << 8;
}

uint16_t CPU::fetch_indirect_X()
{
	printf("%s\n", __func__);
	// TODO: I have no idea if this is right, the description is a bit unclear...
	uint8_t base_address = memory.read(++registers.PC);
	return memory.read(base_address + registers.IX + (registers.PS & PS_CarryFlag));
}

uint16_t CPU::fetch_accumulator()
{
	printf("%s\n", __func__);
	++registers.PC;
	return registers.ACC;
}

uint16_t CPU::fetch_implied()
{
	printf("%s\n", __func__);
	++registers.PC;
	return 0;
}

void CPU::ADC(uint16_t operand)
{
	uint16_t result = registers.ACC + operand + (registers.PS & PS_CarryFlag);

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, result & 0xFF == 0);

	bool overflow = (operand & 0x80 == registers.ACC & 0x80) && ((operand & 0x80 == 0x80) != (result > 0xFF));
	SetProcessorStatus(PS_OverflowFlag, overflow);

	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::ASL(uint16_t operand)
{
	uint16_t result = operand << 1;

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, result & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	// TODO: This can write back to memory if the addressing mode isn't implied.
	// Might be able to solve this by making operand a reference...
	registers.ACC = result & 0xFF;
}

void CPU::CLC(uint16_t operand)
{
	SetProcessorStatus(PS_CarryFlag, false);
}

void CPU::DEY(uint16_t operand)
{
	uint8_t result = registers.IY - 1;

	SetProcessorStatus(PS_ZeroFlag, result & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IY = result;
}

void CPU::LDA(uint16_t operand)
{
	SetProcessorStatus(PS_ZeroFlag, operand & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (operand & 0x80) == 0x80);

	registers.ACC = operand;
}

void CPU::LDX(uint16_t operand)
{
	SetProcessorStatus(PS_ZeroFlag, operand & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (operand & 0x80) == 0x80);

	registers.IX = operand;
}

void CPU::LDY(uint16_t operand)
{
	SetProcessorStatus(PS_ZeroFlag, operand & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (operand & 0x80) == 0x80);

	registers.IY = operand;
}

void CPU::ORA(uint16_t operand)
{
	uint8_t result = registers.ACC | (operand & 0xFF);

	SetProcessorStatus(PS_ZeroFlag, result & 0xFF == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result;
}

void CPU::STX(uint16_t operand)
{
	memory.write(operand, registers.IX);
}

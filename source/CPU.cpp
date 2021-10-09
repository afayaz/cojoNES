#include "CPU.hpp"

#include <stdio.h>

bool CPU::Process()
{
	bool shouldContinue = true;

	// TODO: Fetch the next opcode from memory...
	Opcodes opcode = Opcodes::ADC_absolute;
	auto opcodeIter = opTable.find(opcode);
	if (opcodeIter != opTable.end())
	{
		auto& OpFuncs = opcodeIter->second;
		auto fetchFunc = std::bind(OpFuncs.fetchFunc, this);
		auto opFunc = std::bind(OpFuncs.opFunc, this, std::placeholders::_1);

		uint8_t operand = fetchFunc();
		opFunc(operand);
	}
	else
	{
		printf("Unimplemented opcode %#02x\n", opcode);
		shouldContinue = false;
	}

	return shouldContinue;
}

void CPU::ADC(uint8_t operand)
{
	uint16_t result = registers.ACC + operand + (registers.PS & PS_CarryFlag);

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, result & 0xFF == 0);

	bool overflow = (operand & 0x80 == registers.ACC & 0x80) && ((operand & 0x80 == 0x80) != (result > 0xFF));
	SetProcessorStatus(PS_OverflowFlag, overflow);

	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

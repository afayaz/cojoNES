#include "CPU.hpp"

#include <cstdio>
#include <string>

#include "System.hpp"

// Debug helper function
std::string OpcodeToString(Opcodes opcode)
{
	std::string result;

	switch (opcode)
	{
		case Opcodes::LDX_immediate:
			result = "LDX_immediate";
			break;
		case Opcodes::STX_absolute:
			result = "STX_absolute";
			break;
		case Opcodes::LDY_absolute:
			result = "LDY_absolute";
			break;
		case Opcodes::LDA_immediate:
			result = "LDA_immediate";
			break;
		case Opcodes::CLC:
			result = "CLC";
			break;
		case Opcodes::ADC_absolute:
			result = "ADC_absolute";
			break;
		case Opcodes::DEY:
			result = "DEY";
			break;
		case Opcodes::BNE_relative:
			result = "BNE_relative";
			break;
		case Opcodes::STA_absolute:
			result = "STA_absolute";
			break;
		default:
			result = "unknown";
			break;
	}

	return result;
}

void CPU::ConnectSystem(std::shared_ptr<System> system)
{
	mSystem = system;
}

void CPU::Reset()
{
	// Use uint16_t to ensure bit shifts don't wrap.
	uint16_t PC_lo = mSystem->Read(0xFFFC);
	uint16_t PC_hi = mSystem->Read(0xFFFD);

	registers.PC = PC_lo | PC_hi << 8;

	registers.SP = 0xFD;
}

bool CPU::Process()
{
	bool shouldContinue = true;

	printf("PC is %#02X\n", registers.PC);
	Opcodes opcode = static_cast<Opcodes>(mSystem->Read(registers.PC));
	printf("Executing opcode %s (%#02X)\n", OpcodeToString(opcode).c_str(), static_cast<uint8_t>(opcode));
	auto opcodeIter = opTable.find(opcode);
	if (opcodeIter != opTable.end())
	{
		auto& OpFuncs = opcodeIter->second;
		auto fetchFunc = std::bind(OpFuncs.fetchFunc, this);
		auto opFunc = std::bind(OpFuncs.opFunc, this, std::placeholders::_1);

		DecodedOperand operand = fetchFunc();
		opFunc(operand);

		++registers.PC;

		printf("Registers: ACC = %#02X IX = %#02X IY = %#02X, PC = %#02X, PS = %#02X, SP = %#02X\n", registers.ACC, registers.IX, registers.IY, registers.PC, registers.PS, registers.SP);
		if (opcode == Opcodes::BRK)
		{
			shouldContinue = false;
		}
	}
	else
	{
		printf("Illegal opcode %#02X\n", opcode);
		shouldContinue = false;
	}

	return shouldContinue;
}

CPU::DecodedOperand CPU::fetch_immediate()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC);
	decoded.operandType = OT_Value;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_zeropage()
{
	printf("%s\n", __func__);
	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC);
	decoded.operandType = OT_Value;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_absolute()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	uint16_t lo = mSystem->Read(++registers.PC);
	uint16_t hi = mSystem->Read(++registers.PC);

	decoded.operand = lo | hi << 8;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_absolute_X()
{
	printf("%s\n", __func__);
	DecodedOperand decoded;

	uint16_t lo = mSystem->Read(++registers.PC);
	uint16_t hi = mSystem->Read(++registers.PC);

	uint8_t baseAddress = lo | hi << 8;
	decoded.operand = mSystem->Read(baseAddress + registers.IX);
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_indirect_X()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	// TODO: I have no idea if this is right, the description is a bit unclear...
	uint8_t baseAddress = mSystem->Read(++registers.PC);
	decoded.operand = mSystem->Read(baseAddress + registers.IX + (registers.PS & PS_CarryFlag));
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_accumulator()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	decoded.operand = registers.ACC;
	decoded.operandType = OT_Value;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_relative()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	// Don't increment the PC here, so the offset below is correct.
	int16_t relativeAddress = mSystem->Read(registers.PC + 1);
	if (relativeAddress > 0x80)
	{
		relativeAddress -= 0xFF;
	}

	decoded.operand = registers.PC + relativeAddress;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_implied()
{
	printf("%s\n", __func__);
	return DecodedOperand();
}

void CPU::ADC(DecodedOperand decoded)
{
	printf("%s\n", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}
	
	uint16_t result = registers.ACC + value + (registers.PS & PS_CarryFlag);

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);

	bool overflow = ((value & 0x80) == (registers.ACC & 0x80)) && (((value & 0x80) == 0x80) != (result > 0xFF));
	SetProcessorStatus(PS_OverflowFlag, overflow);

	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::ASL(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint16_t result = decoded.operand << 1;

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	// TODO: This can write back to memory if the addressing mode isn't implied.
	// Might be able to solve this by making operand a reference...
	registers.ACC = result & 0xFF;
}

void CPU::BNE(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	if (!GetProcessorStatus(PS_ZeroFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BPL(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	if (GetProcessorStatus(PS_NegativeFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BRK(DecodedOperand decoded)
{
	// For now, this will halt execution. See CPU::Process().
	printf("%s\n", __func__);
}

void CPU::CLC(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_CarryFlag, false);
}

void CPU::CLD(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_DecimalMode, false);
}

void CPU::CPX(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.IX - decoded.operand;

	SetProcessorStatus(PS_CarryFlag, result >= 0);
	SetProcessorStatus(PS_ZeroFlag, result == 0);
	SetProcessorStatus(PS_NegativeFlag, result >= 0x80);
}

void CPU::DEX(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.IX - 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IX = result;
}

void CPU::DEY(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.IY - 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IY = result;
}

void CPU::INX(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.IX + 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IX = result;
}

void CPU::JSR(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	mSystem->Write(0x100 + registers.SP--, (registers.PC - 1) >> 8);
	mSystem->Write(0x100 + registers.SP--, registers.PC  - 1);
	registers.PC = decoded.operand;
}

void CPU::LDA(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_ZeroFlag, (decoded.operand & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (decoded.operand & 0x80) == 0x80);

	registers.ACC = static_cast<uint8_t>(decoded.operand);
}

void CPU::LDX(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_ZeroFlag, (decoded.operand & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (decoded.operand & 0x80) == 0x80);

	registers.IX = static_cast<uint8_t>(decoded.operand);
}

void CPU::LDY(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_ZeroFlag, (decoded.operand & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (decoded.operand & 0x80) == 0x80);

	uint8_t value = mSystem->Read(decoded.operand);

	registers.IY = value;
}

void CPU::NOP(DecodedOperand decoded)
{
	printf("%s\n", __func__);
}

void CPU::ORA(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.ACC | (decoded.operand & 0xFF);

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result;
}

void CPU::RTS(DecodedOperand decoded)
{
	printf("%s\n", __func__);

	uint16_t returnAddress = mSystem->Read(registers.SP--);
	registers.PC = returnAddress;
}

void CPU::SEI(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_InterruptDisable, true);
}

void CPU::STA(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	mSystem->Write(decoded.operand, registers.ACC);
}

void CPU::STX(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	mSystem->Write(decoded.operand, registers.IX);
}

void CPU::STY(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	mSystem->Write(decoded.operand, registers.IY);
}

void CPU::TXS(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	mSystem->Write(decoded.operand, registers.SP);
}


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
	Reset();
}

void CPU::Reset()
{
	// TODO: Temp data in memory to get started...
	// This is a program to multiply 10 by 3.
	// Taken from part 2 of OLCs NES emulator series
	uint16_t write_addr = 0x8000;
	mSystem->Write(write_addr++, 0xA2); // LDX_immediate
	mSystem->Write(write_addr++, 0x0A); // literal 10
	mSystem->Write(write_addr++, 0x8E); // STX_absolute
	mSystem->Write(write_addr++, 0x00); // Memory offset 0x00
	mSystem->Write(write_addr++, 0x00); // Memory page 0x00
	mSystem->Write(write_addr++, 0xA2); // LDX_immediate
	mSystem->Write(write_addr++, 0x03); // literal 3
	mSystem->Write(write_addr++, 0x8E); // STX_absolute
	mSystem->Write(write_addr++, 0x01); // Memory offset 0x01
	mSystem->Write(write_addr++, 0x00); // Memory page 0x00
	mSystem->Write(write_addr++, 0xAC); // LDY_absolute
	mSystem->Write(write_addr++, 0x00); // Memory offset 0x00
	mSystem->Write(write_addr++, 0x00); // Memory page 0x00
	mSystem->Write(write_addr++, 0xA9); // LDA_immediate
	mSystem->Write(write_addr++, 0x00); // literal 0
	mSystem->Write(write_addr++, 0x18); // CLC
	mSystem->Write(write_addr++, 0x6D); // ADC_absolute
	mSystem->Write(write_addr++, 0x01); // Memory offset 0x01
	mSystem->Write(write_addr++, 0x00); // Memory page 0x00
	mSystem->Write(write_addr++, 0x88); // DEY
	mSystem->Write(write_addr++, 0xD0); // BNE_relative
	mSystem->Write(write_addr++, 0xFA); // literal -5
	mSystem->Write(write_addr++, 0x8D); // STA_absolute
	mSystem->Write(write_addr++, 0x02); // Memory offset 0x02
	mSystem->Write(write_addr++, 0x00); // Memory page 0x00
	mSystem->Write(write_addr++, 0xEA); // NOP
	mSystem->Write(write_addr++, 0xEA); // NOP
	mSystem->Write(write_addr++, 0xEA); // NOP

	mSystem->Write(0xFFFC, 0x00);
	mSystem->Write(0xFFFD, 0x80);

	// Use uint16_t to ensure bit shifts don't wrap.
	uint16_t PC_lo = mSystem->Read(0xFFFC);
	uint16_t PC_hi = mSystem->Read(0xFFFD);

	registers.PC = PC_lo | PC_hi << 8;
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

CPU::DecodedOperand CPU::fetch_indirect_X()
{
	printf("%s\n", __func__);

	DecodedOperand decoded;

	// TODO: I have no idea if this is right, the description is a bit unclear...
	uint8_t base_address = mSystem->Read(++registers.PC);
	decoded.operand = mSystem->Read(base_address + registers.IX + (registers.PS & PS_CarryFlag));
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
	int16_t relative_address = mSystem->Read(registers.PC + 1);
	if (relative_address > 0x80)
	{
		relative_address -= 0xFF;
	}

	decoded.operand = registers.PC + relative_address;
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

void CPU::BRK(DecodedOperand decoded)
{
	printf("%s\n", __func__);

#if __has_builtin(__builtin_debugtrap)
	__builtin_debugtrap();
#elif defined(_MSC_VER)
	__debugbreak();
#else
	// This is noreturn, so can screw up debugging, but should be ok here...
	__builtin_trap();
#endif // 
}

void CPU::CLC(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	SetProcessorStatus(PS_CarryFlag, false);
}

void CPU::DEY(DecodedOperand decoded)
{
	printf("%s\n", __func__);
	uint8_t result = registers.IY - 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IY = result;
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

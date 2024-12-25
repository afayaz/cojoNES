#include "CPU.hpp"

#include <spdlog/spdlog.h>

#include "System.hpp"

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

	SPDLOG_INFO("PC is {:#06x}", registers.PC);
	Opcodes opcode = static_cast<Opcodes>(mSystem->Read(registers.PC));
	mCurrentOpcode = opcode;
	SPDLOG_INFO("Executing opcode {} ({:#04x})", OpcodeToString(opcode), static_cast<uint8_t>(opcode));
	auto opcodeIter = opTable.find(opcode);
	if (opcodeIter != opTable.end())
	{
		auto& OpFuncs = opcodeIter->second;
		auto fetchFunc = std::bind(OpFuncs.fetchFunc, this);
		auto opFunc = std::bind(OpFuncs.opFunc, this, std::placeholders::_1);

		DecodedOperand operand = fetchFunc();
		mCurrentOperand = operand;
		opFunc(operand);

		++registers.PC;

		SPDLOG_INFO("Registers: ACC = {:#04x} IX = {:#04x} IY = {:#04x}, PC = {:#06x}, PS = {:#04x}, SP = {:#04x}", registers.ACC, registers.IX, registers.IY, registers.PC, registers.PS, registers.SP);
		if (opcode == Opcodes::BRK)
		{
			shouldContinue = false;
		}
	}
	else
	{
		SPDLOG_ERROR("Illegal opcode {:#04x}", static_cast<uint8_t>(opcode));
		shouldContinue = false;
	}

	return shouldContinue;
}

CPU::DecodedOperand CPU::fetch_immediate()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC);
	decoded.operandType = OT_Value;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_zeropage()
{
	SPDLOG_INFO("{}", __func__);
	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC);
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_zeropage_X()
{
	SPDLOG_INFO("{}", __func__);
	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC) + registers.IX;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_zeropage_Y()
{
	SPDLOG_INFO("{}", __func__);
	DecodedOperand decoded;

	decoded.operand = mSystem->Read(++registers.PC) + registers.IY;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_absolute()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	uint16_t lo = mSystem->Read(++registers.PC);
	uint16_t hi = mSystem->Read(++registers.PC);

	decoded.operand = lo | hi << 8;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_absolute_X()
{
	SPDLOG_INFO("{}", __func__);
	DecodedOperand decoded;

	uint16_t lo = mSystem->Read(++registers.PC);
	uint16_t hi = mSystem->Read(++registers.PC);

	uint8_t baseAddress = lo | hi << 8;
	decoded.operand = baseAddress + registers.IX;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_absolute_Y()
{
	SPDLOG_INFO("{}", __func__);
	DecodedOperand decoded;

	uint16_t lo = mSystem->Read(++registers.PC);
	uint16_t hi = mSystem->Read(++registers.PC);

	uint8_t baseAddress = lo | hi << 8;
	decoded.operand = baseAddress + registers.IY;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_indirect()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	uint16_t baseAddress_lo = mSystem->Read(++registers.PC);
	uint16_t baseAddress_hi = mSystem->Read(++registers.PC);

	uint16_t baseAddress = baseAddress_lo | baseAddress_hi << 8;
	
	uint16_t indirectAddress_lo = mSystem->Read(baseAddress);
	uint16_t indirectAddress_hi = mSystem->Read(++baseAddress);

	uint16_t indirectAddress = indirectAddress_lo | indirectAddress_hi << 8;

	decoded.operand = indirectAddress;
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_indirect_X()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	// TODO: I have no idea if this is right, the description is a bit unclear...
	uint8_t baseAddress = mSystem->Read(++registers.PC);
	decoded.operand = mSystem->Read(baseAddress + registers.IX + (registers.PS & PS_CarryFlag));
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_indirect_Y()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	// TODO: No idea if this is right, copied from fetch_indirect_X()...
	uint8_t baseAddress = mSystem->Read(++registers.PC);
	decoded.operand = mSystem->Read(baseAddress + registers.IY + (registers.PS & PS_CarryFlag));
	decoded.operandType = OT_Address;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_accumulator()
{
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	decoded.operand = registers.ACC;
	decoded.operandType = OT_Value;

	return decoded;
}

CPU::DecodedOperand CPU::fetch_relative()
{
	SPDLOG_INFO("{}", __func__);

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
	SPDLOG_INFO("{}", __func__);

	DecodedOperand decoded;

	decoded.operand = 0;
	decoded.operandType = OT_Value;

	return decoded;
}

void CPU::ADC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

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

	bool lhsNeg = (registers.ACC & 0x80) == 0x80;
	bool rhsNeg = (value & 0x80) == 0x80;
	bool resultNeg = (result & 0x80) == 0x80;

	bool overflow = false;
	if (lhsNeg && rhsNeg)
	{
		overflow = !resultNeg;
	}
	else if (!lhsNeg && !rhsNeg)
	{
		overflow = resultNeg;
	}
	SetProcessorStatus(PS_OverflowFlag, overflow);

	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::AND(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint16_t result = registers.ACC & value;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::ASL(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint16_t result = value << 1;

	SetProcessorStatus(PS_CarryFlag, result > 0xFF);
	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	if (decoded.operandType == OT_Address)
	{
		mSystem->Write(decoded.operand, result & 0xFF);
	}
	else
	{
		registers.ACC = result & 0xFF;
	}
}

void CPU::BCC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (!GetProcessorStatus(PS_CarryFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BCS(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (GetProcessorStatus(PS_CarryFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BEQ(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (GetProcessorStatus(PS_ZeroFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BIT(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	// This is only supported with Absolute and Zero Page addressing.
	uint8_t result = mSystem->Read(decoded.operand);
	result = registers.ACC & result;

	SetProcessorStatus(PS_ZeroFlag, result == 0x00);
	SetProcessorStatus(PS_OverflowFlag, (result & 0x40) == 0x40);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);
}

void CPU::BMI(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (GetProcessorStatus(PS_NegativeFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BNE(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
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
	SPDLOG_INFO("{}", __func__);
	if (!GetProcessorStatus(PS_NegativeFlag))
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
	SPDLOG_INFO("{}", __func__);
}

void CPU::BVC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (!GetProcessorStatus(PS_OverflowFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::BVS(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	if (GetProcessorStatus(PS_OverflowFlag))
	{
		registers.PC = decoded.operand;
	}
	else
	{
		++registers.PC;
	}
}

void CPU::CLC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_CarryFlag, false);
}

void CPU::CLD(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_DecimalMode, false);
}

void CPU::CLI(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_InterruptDisable, false);
}

void CPU::CLV(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_OverflowFlag, false);
}

void CPU::CMP(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}
	uint16_t result = registers.ACC - value;

	SetProcessorStatus(PS_CarryFlag, result >= 0);
	SetProcessorStatus(PS_ZeroFlag, result == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);
}

void CPU::CPX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}
	uint16_t result = registers.IX - value;

	SetProcessorStatus(PS_CarryFlag, result >= 0);
	SetProcessorStatus(PS_ZeroFlag, result == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);
}

void CPU::CPY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}
	uint16_t result = registers.IY - value;

	SetProcessorStatus(PS_CarryFlag, result >= 0);
	SetProcessorStatus(PS_ZeroFlag, result == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);
}

void CPU::DEC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = mSystem->Read(decoded.operand);
	--result;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	mSystem->Write(decoded.operand, result & 0xFF);
}

void CPU::DEX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = registers.IX - 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IX = result;
}

void CPU::DEY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = registers.IY - 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IY = result;
}

void CPU::EOR(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint16_t result = registers.ACC ^ value;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::INC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = mSystem->Read(decoded.operand);
	++result;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	mSystem->Write(decoded.operand, result & 0xFF);
}

void CPU::INX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = registers.IX + 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IX = result;
}

void CPU::INY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t result = registers.IY + 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.IY = result;
}

void CPU::JMP(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.PC = decoded.operand - 1;
}

void CPU::JSR(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(0x100 + registers.SP--, (registers.PC - 1) >> 8);
	mSystem->Write(0x100 + registers.SP--, registers.PC  - 1);
	registers.PC = decoded.operand;
}

void CPU::LDA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	SetProcessorStatus(PS_ZeroFlag, (value & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (value & 0x80) == 0x80);

	registers.ACC = value;
}

void CPU::LDX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	SetProcessorStatus(PS_ZeroFlag, (value & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (value & 0x80) == 0x80);

	registers.IX = value;
}

void CPU::LDY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	SetProcessorStatus(PS_ZeroFlag, (value & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (value & 0x80) == 0x80);

	registers.IY = value;
}

void CPU::LSR(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	SetProcessorStatus(PS_CarryFlag, (value & 0x01) == 0x01);

	uint16_t result = value >> 1;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	if (decoded.operandType == OT_Address)
	{
		mSystem->Write(decoded.operand, result & 0xFF);
	}
	else
	{
		registers.ACC = result & 0xFF;
	}
}

void CPU::NOP(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
}

void CPU::ORA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint8_t result = registers.ACC | (value & 0xFF);

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result;
}

void CPU::PHA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(0x100 + registers.SP--, registers.ACC);
}

void CPU::PHP(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(0x100 + registers.SP--, registers.PS);
}

void CPU::PLA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.ACC = mSystem->Read(0x100 + ++registers.SP);
}

void CPU::PLP(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.PS = mSystem->Read(0x100 + ++registers.SP);
}

void CPU::ROL(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint8_t oldCarry = static_cast<uint8_t>(GetProcessorStatus(PS_CarryFlag));
	SetProcessorStatus(PS_CarryFlag, (value & 0x80) == 0x80);

	uint16_t result = (value << 1) | oldCarry;

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	if (decoded.operandType == OT_Address)
	{
		mSystem->Write(decoded.operand, result & 0xFF);
	}
	else
	{
		registers.ACC = result & 0xFF;
	}
}

void CPU::ROR(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint8_t oldCarry = static_cast<uint8_t>(GetProcessorStatus(PS_CarryFlag));
	SetProcessorStatus(PS_CarryFlag, (value & 0x01) == 0x01);

	uint16_t result = (oldCarry << 7) | (value >> 1);

	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0x00);
	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	if (decoded.operandType == OT_Address)
	{
		mSystem->Write(decoded.operand, result & 0xFF);
	}
	else
	{
		registers.ACC = result & 0xFF;
	}
}

void CPU::RTI(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	registers.PS = mSystem->Read(0x100 + registers.SP++);
}

void CPU::RTS(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t lo = mSystem->Read(0x100 + registers.SP++);
	uint8_t hi = mSystem->Read(0x100 + registers.SP++);

	registers.PC = lo | hi << 8;
}

void CPU::SBC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);

	uint8_t value = 0;
	if (decoded.operandType == OT_Address)
	{
		value = mSystem->Read(decoded.operand);
	}
	else
	{
		value = decoded.operand;
	}

	uint8_t carry = static_cast<uint8_t>(!GetProcessorStatus(PS_CarryFlag));
	uint16_t result = registers.ACC - value - carry;

	SetProcessorStatus(PS_CarryFlag, result <= 0xFF);
	SetProcessorStatus(PS_ZeroFlag, (result & 0xFF) == 0);

	bool overflow = ((value & 0x80) == (registers.ACC & 0x80)) && (((value & 0x80) == 0x80) != (result > 0xFF));
	SetProcessorStatus(PS_OverflowFlag, overflow);

	SetProcessorStatus(PS_NegativeFlag, (result & 0x80) == 0x80);

	registers.ACC = result & 0xFF;
}

void CPU::SEC(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_CarryFlag, true);
}

void CPU::SED(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_DecimalMode, true);
}

void CPU::SEI(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	SetProcessorStatus(PS_InterruptDisable, true);
}

void CPU::STA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(decoded.operand, registers.ACC);
}

void CPU::STX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(decoded.operand, registers.IX);
}

void CPU::STY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	mSystem->Write(decoded.operand, registers.IY);
}

void CPU::TAX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.IX = registers.ACC;

	SetProcessorStatus(PS_ZeroFlag, registers.IX == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.IX & 0x80) == 0x80);
}

void CPU::TAY(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.IY = registers.ACC;

	SetProcessorStatus(PS_ZeroFlag, registers.IY == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.IY & 0x80) == 0x80);
}

void CPU::TSX(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.IX = registers.SP;

	SetProcessorStatus(PS_ZeroFlag, registers.IX == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.IX & 0x80) == 0x80);
}

void CPU::TXA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.ACC = registers.IX;

	SetProcessorStatus(PS_ZeroFlag, registers.ACC == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.ACC & 0x80) == 0x80);
}

void CPU::TXS(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.SP = registers.IX;

	SetProcessorStatus(PS_ZeroFlag, registers.SP == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.SP & 0x80) == 0x80);
}

void CPU::TYA(DecodedOperand decoded)
{
	SPDLOG_INFO("{}", __func__);
	registers.ACC = registers.IY;

	SetProcessorStatus(PS_ZeroFlag, registers.ACC == 0);
	SetProcessorStatus(PS_CarryFlag, (registers.ACC & 0x80) == 0x80);
}

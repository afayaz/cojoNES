#pragma once

#include <cstdint>
#include <functional>
#include <map>

#include "Opcodes.hpp"
#include "Memory.hpp"

enum ProcessorStatus : uint8_t
{
	CarryFlag = 1,
	ZeroFlag = 2,
	InterruptDisable = 4,
	DecimalMode = 8,
	BreakCommand = 16,
	OverflowFlag = 32,
	NegativeFlag = 64
};

struct CPURegisters
{
	uint16_t PC;
	uint8_t SP;
	uint8_t ACC;
	uint8_t IX;
	uint8_t IY;
	uint8_t PS;
};

class CPU
{
public:
    bool Process();
private:
	void ADC(uint8_t operand);
	void AND(uint8_t operand) {}
	void ASL(uint8_t operand) {}
	void BCC(uint8_t operand) {}
	void BCS(uint8_t operand) {}
	void BEQ(uint8_t operand) {}
	void BIT(uint8_t operand) {}
	void BMI(uint8_t operand) {}
	void BNE(uint8_t operand) {}
	void BPL(uint8_t operand) {}
	void BRK(uint8_t operand) {}
	void BVC(uint8_t operand) {}
	void BVS(uint8_t operand) {}
	void CLC(uint8_t operand) {}
	void CLD(uint8_t operand) {}
	void CLI(uint8_t operand) {}
	void CLV(uint8_t operand) {}
	void CMP(uint8_t operand) {}
	void CPX(uint8_t operand) {}
	void CPY(uint8_t operand) {}
	void DEC(uint8_t operand) {}
	void DEX(uint8_t operand) {}
	void DEY(uint8_t operand) {}
	void EOR(uint8_t operand) {}
	void INC(uint8_t operand) {}
	void INX(uint8_t operand) {}
	void INY(uint8_t operand) {}
	void JMP(uint8_t operand) {}
	void JSR(uint8_t operand) {}
	void LDA(uint8_t operand) {}
	void LDX(uint8_t operand) {}
	void LDY(uint8_t operand) {}
	void LSR(uint8_t operand) {}
	void NOP(uint8_t operand) {}
	void ORA(uint8_t operand) {}
	void PHA(uint8_t operand) {}
	void PHP(uint8_t operand) {}
	void PLA(uint8_t operand) {}
	void PLP(uint8_t operand) {}
	void ROL(uint8_t operand) {}
	void ROR(uint8_t operand) {}
	void RTI(uint8_t operand) {}
	void RTS(uint8_t operand) {}
	void SBC(uint8_t operand) {}
	void SEC(uint8_t operand) {}
	void SED(uint8_t operand) {}
	void SEI(uint8_t operand) {}
	void STA(uint8_t operand) {}
	void STX(uint8_t operand) {}
	void STY(uint8_t operand) {}
	void TAX(uint8_t operand) {}
	void TAY(uint8_t operand) {}
	void TSX(uint8_t operand) {}
	void TXA(uint8_t operand) {}
	void TXS(uint8_t operand) {}
	void TYA(uint8_t operand) {}

private:
	uint8_t fetch_immediate()
	{
		return 0;
	}

	uint8_t fetch_zeropage()
	{
		return 0;
	}

	uint8_t fetch_zeropage_X()
	{
		return 0;
	}

	uint8_t fetch_absolute()
	{
		return 0;
	}

	uint8_t fetch_absolute_X()
	{
		return 0;
	}

	uint8_t fetch_absolute_Y()
	{
		return 0;
	}

	uint8_t fetch_indirect_X()
	{
		return 0;
	}

	uint8_t fetch_indirect_Y()
	{
		return 0;
	}

	// TODO: Check whether these are valid...
	uint8_t fetch_accumulator()
	{
		return 0;
	}

	uint8_t fetch_relative()
	{
		return 0;
	}

	uint8_t fetch_implied() { return 0; }
	uint8_t fetch_indirect() { return 0; }
	uint8_t fetch_zeropage_Y() { return 0; }

	CPURegisters registers = { 0 };

	void SetProcessorStatus(ProcessorStatus statusFlag, bool set)
	{
		if (set)
		{
			registers.PS = registers.PS | statusFlag;
		}
		else
		{
			registers.PS = registers.PS & ~statusFlag;
		}
	}

	struct OpFuncs
	{
		std::function<uint8_t(CPU*)> fetchFunc;
		std::function<void(CPU*, uint8_t)> opFunc;
	};

	const std::map<const Opcodes, const OpFuncs> opTable
	{
		{ Opcodes::ADC_immediate, {&CPU::fetch_immediate, &CPU::ADC} },
		{ Opcodes::ADC_zeropage, {&CPU::fetch_zeropage, &CPU::ADC} },
		{ Opcodes::ADC_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::ADC} },
		{ Opcodes::ADC_absolute, {&CPU::fetch_absolute, &CPU::ADC} },
		{ Opcodes::ADC_absolute_X, {&CPU::fetch_absolute_X, &CPU::ADC} },
		{ Opcodes::ADC_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::ADC} },
		{ Opcodes::ADC_indirect_X, {&CPU::fetch_indirect_X, &CPU::ADC} },
		{ Opcodes::ADC_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::ADC} },

		{ Opcodes::AND_immediate, {&CPU::fetch_immediate, &CPU::AND} },
		{ Opcodes::AND_zeropage, {&CPU::fetch_zeropage, &CPU::AND} },
		{ Opcodes::AND_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::AND} },
		{ Opcodes::AND_absolute, {&CPU::fetch_absolute, &CPU::AND} },
		{ Opcodes::AND_absolute_X, {&CPU::fetch_absolute_X, &CPU::AND} },
		{ Opcodes::AND_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::AND} },
		{ Opcodes::AND_indirect_X, {&CPU::fetch_indirect_X, &CPU::AND} },
		{ Opcodes::AND_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::AND} },

		{ Opcodes::ASL_accumulator, {&CPU::fetch_accumulator, &CPU::ASL} },
		{ Opcodes::ASL_zeropage, {&CPU::fetch_zeropage, &CPU::ASL} },
		{ Opcodes::ASL_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::ASL} },
		{ Opcodes::ASL_absolute, {&CPU::fetch_absolute, &CPU::ASL} },
		{ Opcodes::ASL_absolute_X, {&CPU::fetch_absolute_X, &CPU::ASL} },

		{ Opcodes::BCC_relative, {&CPU::fetch_relative, &CPU::BCC} },

		{ Opcodes::BCS_relative, {&CPU::fetch_relative, &CPU::BCS} },

		{ Opcodes::BEQ_relative, {&CPU::fetch_relative, &CPU::BEQ} },

		{ Opcodes::BIT_zeropage, {&CPU::fetch_zeropage, &CPU::BIT} },
		{ Opcodes::BIT_absolute, {&CPU::fetch_absolute, &CPU::BIT} },

		{ Opcodes::BMI_relative, {&CPU::fetch_relative, &CPU::BMI} },

		{ Opcodes::BNE_relative, {&CPU::fetch_relative, &CPU::BNE} },

		{ Opcodes::BPL_relative, {&CPU::fetch_relative, &CPU::BPL} },

		{ Opcodes::BRK, {&CPU::fetch_implied, &CPU::BRK} },

		{ Opcodes::BVC_relative, {&CPU::fetch_relative, &CPU::BVC} },

		{ Opcodes::BVS_relative, {&CPU::fetch_relative, &CPU::BVS} },

		{ Opcodes::CLC, {&CPU::fetch_implied, &CPU::CLC} },

		{ Opcodes::CLD, {&CPU::fetch_implied, &CPU::CLD} },

		{ Opcodes::CLI, {&CPU::fetch_implied, &CPU::CLI} },

		{ Opcodes::CLV, {&CPU::fetch_implied, &CPU::CLV} },

		{ Opcodes::CMP_immediate, {&CPU::fetch_immediate, &CPU::CMP} },
		{ Opcodes::CMP_zeropage, {&CPU::fetch_zeropage, &CPU::CMP} },
		{ Opcodes::CMP_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::CMP} },
		{ Opcodes::CMP_absolute, {&CPU::fetch_absolute, &CPU::CMP} },
		{ Opcodes::CMP_absolute_X, {&CPU::fetch_absolute_X, &CPU::CMP} },
		{ Opcodes::CMP_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::CMP} },
		{ Opcodes::CMP_indirect_X, {&CPU::fetch_indirect_X, &CPU::CMP} },
		{ Opcodes::CMP_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::CMP} },

		{ Opcodes::CPX_immediate, {&CPU::fetch_immediate, &CPU::CPX} },
		{ Opcodes::CPX_zeropage, {&CPU::fetch_zeropage, &CPU::CPX} },
		{ Opcodes::CPX_absolute, {&CPU::fetch_zeropage_X, &CPU::CPX} },

		{ Opcodes::CPY_immediate, {&CPU::fetch_immediate, &CPU::CPY} },
		{ Opcodes::CPY_zeropage, {&CPU::fetch_zeropage, &CPU::CPY} },
		{ Opcodes::CPY_absolute, {&CPU::fetch_absolute, &CPU::CPY} },

		{ Opcodes::DEC_zeropage, {&CPU::fetch_zeropage, &CPU::DEC} },
		{ Opcodes::DEC_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::DEC} },
		{ Opcodes::DEC_absolute, {&CPU::fetch_absolute, &CPU::DEC} },
		{ Opcodes::DEC_absolute_X, {&CPU::fetch_absolute_X, &CPU::DEC} },

		{ Opcodes::DEX, {&CPU::fetch_implied, &CPU::DEX} },
		{ Opcodes::DEY, {&CPU::fetch_implied, &CPU::DEY} },

		{ Opcodes::EOR_immediate, {&CPU::fetch_immediate, &CPU::EOR} },
		{ Opcodes::EOR_zeropage, {&CPU::fetch_zeropage, &CPU::EOR} },
		{ Opcodes::EOR_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::EOR} },
		{ Opcodes::EOR_absolute, {&CPU::fetch_absolute, &CPU::EOR} },
		{ Opcodes::EOR_absolute_X, {&CPU::fetch_absolute, &CPU::EOR} },
		{ Opcodes::EOR_absolute_Y, {&CPU::fetch_absolute, &CPU::EOR} },
		{ Opcodes::EOR_indirect_X, {&CPU::fetch_indirect_X, &CPU::EOR} },
		{ Opcodes::EOR_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::EOR} },

		{ Opcodes::INC_zeropage, {&CPU::fetch_zeropage, &CPU::INC} },
		{ Opcodes::INC_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::INC} },
		{ Opcodes::INC_absolute, {&CPU::fetch_absolute, &CPU::INC} },
		{ Opcodes::INC_absolute_X, {&CPU::fetch_absolute_X, &CPU::INC} },

		{ Opcodes::INX, {&CPU::fetch_implied, &CPU::INX} },

		{ Opcodes::INY, {&CPU::fetch_implied, &CPU::INY} },

		{ Opcodes::JMP_absolute, {&CPU::fetch_absolute, &CPU::JMP} },
		{ Opcodes::JMP_indirect, {&CPU::fetch_indirect, &CPU::JMP} },

		{ Opcodes::JSR, {&CPU::fetch_implied, &CPU::JSR} },

		{ Opcodes::LDA_immediate, {&CPU::fetch_immediate, &CPU::LDA} },
		{ Opcodes::LDA_zeropage, {&CPU::fetch_zeropage, &CPU::LDA} },
		{ Opcodes::LDA_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::LDA} },
		{ Opcodes::LDA_absolute, {&CPU::fetch_absolute, &CPU::LDA} },
		{ Opcodes::LDA_absolute_X, {&CPU::fetch_absolute_X, &CPU::LDA} },
		{ Opcodes::LDA_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::LDA} },
		{ Opcodes::LDA_indirect_X, {&CPU::fetch_indirect_X, &CPU::LDA} },
		{ Opcodes::LDA_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::LDA} },

		{ Opcodes::LDX_immediate, {&CPU::fetch_immediate, &CPU::LDX} },
		{ Opcodes::LDX_zeropage, {&CPU::fetch_zeropage, &CPU::LDX} },
		{ Opcodes::LDX_zeropage_Y, {&CPU::fetch_zeropage_Y, &CPU::LDX} },
		{ Opcodes::LDX_absolute, {&CPU::fetch_absolute, &CPU::LDX} },
		{ Opcodes::LDX_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::LDX} },

		{ Opcodes::LDY_immediate, {&CPU::fetch_immediate, &CPU::LDY} },
		{ Opcodes::LDY_zeropage, {&CPU::fetch_zeropage, &CPU::LDY} },
		{ Opcodes::LDY_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::LDY} },
		{ Opcodes::LDY_absolute, {&CPU::fetch_absolute, &CPU::LDY} },
		{ Opcodes::LDY_absolute_X, {&CPU::fetch_absolute_X, &CPU::LDY} },

		{ Opcodes::LSR_accumulator, {&CPU::fetch_accumulator, &CPU::LSR} },
		{ Opcodes::LSR_zeropage, {&CPU::fetch_zeropage, &CPU::LSR} },
		{ Opcodes::LSR_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::LSR} },
		{ Opcodes::LSR_absolute, {&CPU::fetch_absolute, &CPU::LSR} },
		{ Opcodes::LSR_absolute_X, {&CPU::fetch_absolute_X, &CPU::LSR} },

		{ Opcodes::NOP, {&CPU::fetch_implied, &CPU::NOP} },

		{ Opcodes::ORA_immediate, {&CPU::fetch_immediate, &CPU::ORA} },
		{ Opcodes::ORA_zeropage, {&CPU::fetch_zeropage, &CPU::ORA} },
		{ Opcodes::ORA_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::ORA} },
		{ Opcodes::ORA_absolute, {&CPU::fetch_absolute, &CPU::ORA} },
		{ Opcodes::ORA_absolute_X, {&CPU::fetch_absolute_X, &CPU::ORA} },
		{ Opcodes::ORA_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::ORA} },
		{ Opcodes::ORA_indirect_X, {&CPU::fetch_indirect_X, &CPU::ORA} },
		{ Opcodes::ORA_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::ORA} },

		{ Opcodes::PHA, {&CPU::fetch_implied, &CPU::PHA} },

		{ Opcodes::PHP, {&CPU::fetch_implied, &CPU::PHP} },

		{ Opcodes::PLA, {&CPU::fetch_implied, &CPU::PLA} },

		{ Opcodes::PLP, {&CPU::fetch_implied, &CPU::PLP} },

		{ Opcodes::ROL_accumulator, {&CPU::fetch_implied, &CPU::ROL} },
		{ Opcodes::ROL_zeropage, {&CPU::fetch_zeropage, &CPU::ROL} },
		{ Opcodes::ROL_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::ROL} },
		{ Opcodes::ROL_absolute, {&CPU::fetch_absolute, &CPU::ROL} },
		{ Opcodes::ROL_absolute_X, {&CPU::fetch_absolute_X, &CPU::ROL} },

		{ Opcodes::ROR_accumulator, {&CPU::fetch_implied, &CPU::ROR} },
		{ Opcodes::ROR_zeropage, {&CPU::fetch_zeropage, &CPU::ROR} },
		{ Opcodes::ROR_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::ROR} },
		{ Opcodes::ROR_absolute, {&CPU::fetch_absolute, &CPU::ROR} },
		{ Opcodes::ROR_absolute_X, {&CPU::fetch_absolute_X, &CPU::ROR} },

		{ Opcodes::RTI, {&CPU::fetch_implied, &CPU::RTI} },

		{ Opcodes::RTS, {&CPU::fetch_implied, &CPU::RTS} },

		{ Opcodes::SBC_immediate, {&CPU::fetch_immediate, &CPU::SBC} },
		{ Opcodes::SBC_zeropage, {&CPU::fetch_zeropage, &CPU::SBC} },
		{ Opcodes::SBC_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::SBC} },
		{ Opcodes::SBC_absolute, {&CPU::fetch_absolute, &CPU::SBC} },
		{ Opcodes::SBC_absolute_X, {&CPU::fetch_absolute_X, &CPU::SBC} },
		{ Opcodes::SBC_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::SBC} },
		{ Opcodes::SBC_indirect_X, {&CPU::fetch_indirect_X, &CPU::SBC} },
		{ Opcodes::SBC_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::SBC} },

		{ Opcodes::SEC, {&CPU::fetch_implied, &CPU::SEC} },
		{ Opcodes::SED, {&CPU::fetch_implied, &CPU::SED} },
		{ Opcodes::SEI, {&CPU::fetch_implied, &CPU::SEI} },

		{ Opcodes::STA_zeropage, {&CPU::fetch_zeropage, &CPU::STA} },
		{ Opcodes::STA_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::STA} },
		{ Opcodes::STA_absolute, {&CPU::fetch_absolute, &CPU::STA} },
		{ Opcodes::STA_absolute_X, {&CPU::fetch_absolute_X, &CPU::STA} },
		{ Opcodes::STA_absolute_Y, {&CPU::fetch_absolute_Y, &CPU::STA} },
		{ Opcodes::STA_indirect_X, {&CPU::fetch_indirect_X, &CPU::STA} },
		{ Opcodes::STA_indirect_Y, {&CPU::fetch_indirect_Y, &CPU::STA} },

		{ Opcodes::STX_zeropage, {&CPU::fetch_zeropage, &CPU::STX} },
		{ Opcodes::STX_zeropage_Y, {&CPU::fetch_zeropage_Y, &CPU::STX} },
		{ Opcodes::STX_absolute, {&CPU::fetch_absolute, &CPU::STX} },

		{ Opcodes::STY_zeropage, {&CPU::fetch_zeropage, &CPU::STY} },
		{ Opcodes::STY_zeropage_X, {&CPU::fetch_zeropage_X, &CPU::STY} },
		{ Opcodes::STY_absolute, {&CPU::fetch_absolute, &CPU::STY} },

		{ Opcodes::TAX, {&CPU::fetch_implied, &CPU::TAX} },
		{ Opcodes::TAY, {&CPU::fetch_implied, &CPU::TAY} },
		{ Opcodes::TSX, {&CPU::fetch_implied, &CPU::TSX} },
		{ Opcodes::TXA, {&CPU::fetch_implied, &CPU::TXA} },
		{ Opcodes::TXS, {&CPU::fetch_implied, &CPU::TXS} },
		{ Opcodes::TYA, {&CPU::fetch_implied, &CPU::TYA} },
	};
};
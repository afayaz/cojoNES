#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <cstdio>

#include "Opcodes.hpp"
#include "Memory.hpp"

// TODO: Come up with better logging
#define LOG_UNIMPLEMENTED_OP() printf("Unimplemented op %s \n", __func__)
#define LOG_UNIMPLEMENTED_FETCH() printf("Unimplemented addressing mode %s \n", __func__)

enum ProcessorStatus : uint8_t
{
	PS_CarryFlag        = (1 << 0),
	PS_ZeroFlag         = (1 << 1),
	PS_InterruptDisable = (1 << 2),
	PS_DecimalMode      = (1 << 3),
	PS_BreakCommand     = (1 << 4),
	PS_Ignored          = (1 << 5),
	PS_OverflowFlag     = (1 << 6),
	PS_NegativeFlag     = (1 << 7)
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
	CPU();

	void Reset();
	bool Process();
private:
	void ADC(uint16_t operand);
	void AND(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void ASL(uint16_t operand);
	void BCC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BCS(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BEQ(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BIT(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BMI(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BNE(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BPL(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BRK(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BVC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void BVS(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CLC(uint16_t operand);
	void CLD(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CLI(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CLV(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CMP(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CPX(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void CPY(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void DEC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void DEX(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void DEY(uint16_t operand);
	void EOR(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void INC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void INX(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void INY(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void JMP(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void JSR(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void LDA(uint16_t operand);
	void LDX(uint16_t operand);
	void LDY(uint16_t operand);
	void LSR(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void NOP(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void ORA(uint16_t operand);
	void PHA(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void PHP(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void PLA(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void PLP(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void ROL(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void ROR(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void RTI(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void RTS(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void SBC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void SEC(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void SED(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void SEI(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void STA(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void STX(uint16_t operand);
	void STY(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TAX(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TAY(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TSX(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TXA(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TXS(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }
	void TYA(uint16_t operand) { LOG_UNIMPLEMENTED_OP(); }

private:
	uint16_t fetch_immediate();

	uint16_t fetch_zeropage()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_zeropage_X()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_absolute();

	uint16_t fetch_absolute_X()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_absolute_Y()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_indirect_X();

	uint16_t fetch_indirect_Y()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_accumulator();

	uint16_t fetch_relative()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

	uint16_t fetch_implied();

	uint16_t fetch_indirect()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}
	uint16_t fetch_zeropage_Y()
	{
		LOG_UNIMPLEMENTED_FETCH();
		return 0;
	}

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
		std::function<uint16_t(CPU*)> fetchFunc;
		std::function<void(CPU*, uint16_t)> opFunc;
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

	Memory memory;
};
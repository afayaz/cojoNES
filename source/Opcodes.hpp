#pragma once

// Based on https://www.masswerk.at/6502/6502_instruction_set.html (Appendix A).
enum class Opcodes : uint8_t
{
	// ADC: Add memory to accumulator with carry.
	ADC_immediate = 0x69,
	ADC_zeropage = 0x65,
	ADC_zeropage_X = 0x75,
	ADC_absolute = 0x6D,
	ADC_absolute_X = 0x7D,
	ADC_absolute_Y = 0x79,
	ADC_indirect_X = 0x61,
	ADC_indirect_Y = 0x71,

	// AND: Bitwise AND memory with accumulator.
	AND_immediate = 0x29,
	AND_zeropage = 0x25,
	AND_zeropage_X = 0x35,
	AND_absolute = 0x2D,
	AND_absolute_X = 0x3D,
	AND_absolute_Y = 0x39,
	AND_indirect_X = 0x21,
	AND_indirect_Y = 0x31,

	// ASL: Left shift 1 bit (Memory or Accumulator).
	ASL_accumulator = 0x0A,
	ASL_zeropage = 0x06,
	ASL_zeropage_X = 0x16,
	ASL_absolute = 0x0E,
	ASL_absolute_X = 0x1E,

	// BCC: Branch on carry clear
	BCC_relative = 0x90,

	// BCS: Branch on carry set.
	BCS_relative = 0xB0,

	// BEQ: Branch on Result Zero.
	BEQ_relative = 0xF0,

	// BIT  Test Bits in Memory with Accumulator
	BIT_zeropage = 0x24,
	BIT_absolute = 0x2C,

	// BMI  Branch on Result Minus
	BMI_relative = 0x30,

	// BNE  Branch on Result not Zero
	BNE_relative = 0xD0,

	// BPL  Branch on Result Plus
	BPL_relative = 0x10,

	// BRK  Force Break
	BRK = 0x00,

	// BVC  Branch on Overflow Clear
	BVC_relative = 0x50,

	// BVS  Branch on Overflow Set
	BVS_relative = 0x70,

	// CLC  Clear Carry Flag
	CLC = 0x18,

	// CLD  Clear Decimal Mode
	CLD = 0xD8,

	// CLI  Clear Interrupt Disable Bit
	CLI = 0x58,

	// CLV  Clear Overflow Flag
	CLV = 0xB8,

	// CMP  Compare Memory with Accumulator
	CMP_immediate = 0xC9,
	CMP_zeropage = 0xC5,
	CMP_zeropage_X = 0xD5,
	CMP_absolute = 0xCD,
	CMP_absolute_X = 0xDD,
	CMP_absolute_Y = 0xD9,
	CMP_indirect_X = 0xC1,
	CMP_indirect_Y = 0xD1,

	// CPX  Compare Memory and Index X
	CPX_immediate = 0xE0,
	CPX_zeropage = 0xE4,
	CPX_absolute = 0xEC,

	// CPY  Compare Memory and Index Y
	CPY_immediate = 0xC0,
	CPY_zeropage = 0xC4,
	CPY_absolute = 0xCC,

	// DEC  Decrement Memory by One
	DEC_zeropage = 0xC6,
	DEC_zeropage_X = 0xD6,
	DEC_absolute = 0xCE,
	DEC_absolute_X = 0xDE,

	// DEX  Decrement Index X by One
	// DEY  Decrement Index Y by One
	DEX = 0xCA,
	DEY = 0x88,

	// EOR  Exclusive - OR Memory with Accumulator
	EOR_immediate = 0x49,
	EOR_zeropage = 0x45,
	EOR_zeropage_X = 0x55,
	EOR_absolute = 0x4D,
	EOR_absolute_X = 0x5D,
	EOR_absolute_Y = 0x59,
	EOR_indirect_X = 0x41,
	EOR_indirect_Y = 0x51,

	// INC  Increment Memory by One
	INC_zeropage = 0xE6,
	INC_zeropage_X = 0xF6,
	INC_absolute = 0xEE,
	INC_absolute_X = 0xFE,

	// INX  Increment Index X by One
	INX = 0xE8,

	// INY  Increment Index Y by One
	INY = 0xC8,

	// JMP  Jump to New Location
	JMP_absolute = 0x4C,
	JMP_indirect = 0x6C,

	// JSR  Jump to New Location Saving Return Address
	JSR = 0x20,

	// LDA  Load Accumulator with Memory
	LDA_immediate = 0xA9,
	LDA_zeropage = 0xA5,
	LDA_zeropage_X = 0xB5,
	LDA_absolute = 0xAD,
	LDA_absolute_X = 0xBD,
	LDA_absolute_Y = 0xB9,
	LDA_indirect_X = 0xA1,
	LDA_indirect_Y = 0xB1,

	// LDX  Load Index X with Memory
	LDX_immediate = 0xA2,
	LDX_zeropage = 0xA6,
	LDX_zeropage_Y = 0xB6,
	LDX_absolute = 0xAE,
	LDX_absolute_Y = 0xBE,

	// LDY  Load Index Y with Memory
	LDY_immediate = 0xA0,
	LDY_zeropage = 0xA4,
	LDY_zeropage_X = 0xB4,
	LDY_absolute = 0xAC,
	LDY_absolute_X = 0xBC,

	// LSR  Shift One Bit Right (Memory or Accumulator)
	LSR_accumulator = 0x4A,
	LSR_zeropage = 0x46,
	LSR_zeropage_X = 0x56,
	LSR_absolute = 0x4E,
	LSR_absolute_X = 0x5E,

	// NOP  No Operation
	NOP = 0xEA,

	// ORA  OR Memory with Accumulator
	ORA_immediate = 0x09,
	ORA_zeropage = 0x05,
	ORA_zeropage_X = 0x15,
	ORA_absolute = 0x0D,
	ORA_absolute_X = 0x1D,
	ORA_absolute_Y = 0x19,
	ORA_indirect_X = 0x01,
	ORA_indirect_Y = 0x11,

	// PHA  Push Accumulator on Stack
	PHA = 0x48,

	// PHP  Push Processor Status on Stack
	PHP = 0x08,

	// PLA  Pull Accumulator from Stack
	PLA = 0x68,

	// PLP  Pull Processor Status from Stack
	PLP = 0x28,

	// ROL  Rotate One Bit Left (Memory or Accumulator)
	ROL_accumulator = 0x2A,
	ROL_zeropage = 0x26,
	ROL_zeropage_X = 0x36,
	ROL_absolute = 0x2E,
	ROL_absolute_X = 0x3E,

	// ROR  Rotate One Bit Right (Memory or Accumulator)
	ROR_accumulator = 0x6A,
	ROR_zeropage = 0x66,
	ROR_zeropage_X = 0x76,
	ROR_absolute = 0x6E,
	ROR_absolute_X = 0x7E,

	// RTI  Return from Interrupt
	RTI = 0x40,

	// RTS  Return from Subroutine
	RTS = 0x60,

	// SBC  Subtract Memory from Accumulator with Borrow
	SBC_immediate = 0xE9,
	SBC_zeropage = 0xE5,
	SBC_zeropage_X = 0xF5,
	SBC_absolute = 0xED,
	SBC_absolute_X = 0xFD,
	SBC_absolute_Y = 0xF9,
	SBC_indirect_X = 0xE1,
	SBC_indirect_Y = 0xF1,

	// SEC  Set Carry Flag
	SEC = 0x38,

	// SED  Set Decimal Flag
	SED = 0xF8,

	// SEI  Set Interrupt Disable Status
	SEI = 0x78,

	// STA  Store Accumulator in Memory
	STA_zeropage = 0x85,
	STA_zeropage_X = 0x95,
	STA_absolute = 0x8D,
	STA_absolute_X = 0x9D,
	STA_absolute_Y = 0x99,
	STA_indirect_X = 0x81,
	STA_indirect_Y = 0x91,

	// STX  Store Index X in Memory
	STX_zeropage = 0x86,
	STX_zeropage_Y = 0x96,
	STX_absolute = 0x8E,

	// STY  Sore Index Y in Memory
	STY_zeropage = 0x84,
	STY_zeropage_X = 0x94,
	STY_absolute = 0x8C,

	// TAX  Transfer Accumulator to Index X
	TAX = 0xAA,

	// TAY  Transfer Accumulator to Index Y
	TAY = 0xA8,

	// TSX  Transfer Stack Pointer to Index X
	TSX = 0xBA,

	// TXA  Transfer Index X to Accumulator
	TXA = 0x8A,

	// TXS  Transfer Index X to Stack Register
	TXS = 0x9A,

	// TYA  Transfer Index Y to Accumulator
	TYA = 0x98,
};

// Debug helper function
constexpr const char* OpcodeToString(Opcodes opcode)
{
	const char* result = "";

	switch (opcode)
	{
		case Opcodes::ADC_immediate: result = "ADC_immediate"; break;
		case Opcodes::ADC_zeropage: result = "ADC_zeropage"; break;
		case Opcodes::ADC_zeropage_X: result = "ADC_zeropage_X"; break;
		case Opcodes::ADC_absolute: result = "ADC_absolute"; break;
		case Opcodes::ADC_absolute_X: result = "ADC_absolute_X"; break;
		case Opcodes::ADC_absolute_Y: result = "ADC_absolute_Y"; break;
		case Opcodes::ADC_indirect_X: result = "ADC_indirect_X"; break;
		case Opcodes::ADC_indirect_Y: result = "ADC_indirect_Y"; break;

		case Opcodes::AND_immediate: result = "AND_immediate"; break;
		case Opcodes::AND_zeropage: result = "AND_zeropage"; break;
		case Opcodes::AND_zeropage_X: result = "AND_zeropage_X"; break;
		case Opcodes::AND_absolute: result = "AND_absolute"; break;
		case Opcodes::AND_absolute_X: result = "AND_absolute_X"; break;
		case Opcodes::AND_absolute_Y: result = "AND_absolute_Y"; break;
		case Opcodes::AND_indirect_X: result = "AND_indirect_X"; break;
		case Opcodes::AND_indirect_Y: result = "AND_indirect_Y"; break;

		case Opcodes::ASL_accumulator: result = "ASL_accumulator"; break;
		case Opcodes::ASL_zeropage: result = "ASL_zeropage"; break;
		case Opcodes::ASL_zeropage_X: result = "ASL_zeropage_X"; break;
		case Opcodes::ASL_absolute: result = "ASL_absolute"; break;
		case Opcodes::ASL_absolute_X: result = "ASL_absolute_X"; break;

		case Opcodes::BCC_relative: result = "BCC_relative"; break;

		case Opcodes::BCS_relative: result = "BCS_relative"; break;

		case Opcodes::BEQ_relative: result = "BEQ_relative"; break;

		case Opcodes::BIT_zeropage: result = "BIT_zeropage"; break;
		case Opcodes::BIT_absolute: result = "BIT_absolute"; break;

		case Opcodes::BMI_relative: result = "BMI_relative"; break;

		case Opcodes::BNE_relative: result = "BNE_relative"; break;

		case Opcodes::BPL_relative: result = "BPL_relative"; break;

		case Opcodes::BRK: result = "BRK"; break;

		case Opcodes::BVC_relative: result = "BVC_relative"; break;

		case Opcodes::BVS_relative: result = "BVS_relative"; break;

		case Opcodes::CLC: result = "CLC"; break;

		case Opcodes::CLD: result = "CLD"; break;

		case Opcodes::CLI: result = "CLI"; break;

		case Opcodes::CLV: result = "CLV"; break;

		case Opcodes::CMP_immediate: result = "CMP_immediate"; break;
		case Opcodes::CMP_zeropage: result = "CMP_zeropage"; break;
		case Opcodes::CMP_zeropage_X: result = "CMP_zeropage_X"; break;
		case Opcodes::CMP_absolute: result = "CMP_absolute"; break;
		case Opcodes::CMP_absolute_X: result = "CMP_absolute_X"; break;
		case Opcodes::CMP_absolute_Y: result = "CMP_absolute_Y"; break;
		case Opcodes::CMP_indirect_X: result = "CMP_indirect_X"; break;
		case Opcodes::CMP_indirect_Y: result = "CMP_indirect_Y"; break;

		case Opcodes::CPX_immediate: result = "CPX_immediate"; break;
		case Opcodes::CPX_zeropage: result = "CPX_zeropage"; break;
		case Opcodes::CPX_absolute: result = "CPX_absolute"; break;

		case Opcodes::CPY_immediate: result = "CPY_immediate"; break;
		case Opcodes::CPY_zeropage: result = "CPY_zeropage"; break;
		case Opcodes::CPY_absolute: result = "CPY_absolute"; break;

		case Opcodes::DEC_zeropage: result = "DEC_zeropage"; break;
		case Opcodes::DEC_zeropage_X: result = "DEC_zeropage_X"; break;
		case Opcodes::DEC_absolute: result = "DEC_absolute"; break;
		case Opcodes::DEC_absolute_X: result = "DEC_absolute_X"; break;

		case Opcodes::DEX: result = "DEX"; break;
		case Opcodes::DEY: result = "DEY"; break;

		case Opcodes::EOR_immediate: result = "EOR_immediate"; break;
		case Opcodes::EOR_zeropage: result = "EOR_zeropage"; break;
		case Opcodes::EOR_zeropage_X: result = "EOR_zeropage_X"; break;
		case Opcodes::EOR_absolute: result = "EOR_absolute"; break;
		case Opcodes::EOR_absolute_X: result = "EOR_absolute_X"; break;
		case Opcodes::EOR_absolute_Y: result = "EOR_absolute_Y"; break;
		case Opcodes::EOR_indirect_X: result = "EOR_indirect_X"; break;
		case Opcodes::EOR_indirect_Y: result = "EOR_indirect_Y"; break;

		case Opcodes::INC_zeropage: result = "INC_zeropage"; break;
		case Opcodes::INC_zeropage_X: result = "INC_zeropage_X"; break;
		case Opcodes::INC_absolute: result = "INC_absolute"; break;
		case Opcodes::INC_absolute_X: result = "INC_absolute_X"; break;

		case Opcodes::INX: result = "INX"; break;

		case Opcodes::INY: result = "INY"; break;

		case Opcodes::JMP_absolute: result = "JMP_absolute"; break;
		case Opcodes::JMP_indirect: result = "JMP_indirect"; break;

		case Opcodes::JSR: result = "JSR"; break;

		case Opcodes::LDA_immediate: result = "LDA_immediate"; break;
		case Opcodes::LDA_zeropage: result = "LDA_zeropage"; break;
		case Opcodes::LDA_zeropage_X: result = "LDA_zeropage_X"; break;
		case Opcodes::LDA_absolute: result = "LDA_absolute"; break;
		case Opcodes::LDA_absolute_X: result = "LDA_absolute_X"; break;
		case Opcodes::LDA_absolute_Y: result = "LDA_absolute_Y"; break;
		case Opcodes::LDA_indirect_X: result = "LDA_indirect_X"; break;
		case Opcodes::LDA_indirect_Y: result = "LDA_indirect_Y"; break;

		case Opcodes::LDX_immediate: result = "LDX_immediate"; break;
		case Opcodes::LDX_zeropage: result = "LDX_zeropage"; break;
		case Opcodes::LDX_zeropage_Y: result = "LDX_zeropage_Y"; break;
		case Opcodes::LDX_absolute: result = "LDX_absolute"; break;
		case Opcodes::LDX_absolute_Y: result = "LDX_absolute_Y"; break;

		case Opcodes::LDY_immediate: result = "LDY_immediate"; break;
		case Opcodes::LDY_zeropage: result = "LDY_zeropage"; break;
		case Opcodes::LDY_zeropage_X: result = "LDY_zeropage_X"; break;
		case Opcodes::LDY_absolute: result = "LDY_absolute"; break;
		case Opcodes::LDY_absolute_X: result = "LDY_absolute_X"; break;

		case Opcodes::LSR_accumulator: result = "LSR_accumulator"; break;
		case Opcodes::LSR_zeropage: result = "LSR_zeropage"; break;
		case Opcodes::LSR_zeropage_X: result = "LSR_zeropage_X"; break;
		case Opcodes::LSR_absolute: result = "LSR_absolute"; break;
		case Opcodes::LSR_absolute_X: result = "LSR_absolute_X"; break;

		case Opcodes::NOP: result = "NOP"; break;

		case Opcodes::ORA_immediate: result = "ORA_immediate"; break;
		case Opcodes::ORA_zeropage: result = "ORA_zeropage"; break;
		case Opcodes::ORA_zeropage_X: result = "ORA_zeropage_X"; break;
		case Opcodes::ORA_absolute: result = "ORA_absolute"; break;
		case Opcodes::ORA_absolute_X: result = "ORA_absolute_X"; break;
		case Opcodes::ORA_absolute_Y: result = "ORA_absolute_Y"; break;
		case Opcodes::ORA_indirect_X: result = "ORA_indirect_X"; break;
		case Opcodes::ORA_indirect_Y: result = "ORA_indirect_Y"; break;

		case Opcodes::PHA: result = "PHA"; break;

		case Opcodes::PHP: result = "PHP"; break;

		case Opcodes::PLA: result = "PLA"; break;

		case Opcodes::PLP: result = "PLP"; break;

		case Opcodes::ROL_accumulator: result = "ROL_accumulator"; break;
		case Opcodes::ROL_zeropage: result = "ROL_zeropage"; break;
		case Opcodes::ROL_zeropage_X: result = "ROL_zeropage_X"; break;
		case Opcodes::ROL_absolute: result = "ROL_absolute"; break;
		case Opcodes::ROL_absolute_X: result = "ROL_absolute_X"; break;

		case Opcodes::ROR_accumulator: result = "ROR_accumulator"; break;
		case Opcodes::ROR_zeropage: result = "ROR_zeropage"; break;
		case Opcodes::ROR_zeropage_X: result = "ROR_zeropage_X"; break;
		case Opcodes::ROR_absolute: result = "ROR_absolute"; break;
		case Opcodes::ROR_absolute_X: result = "ROR_absolute_X"; break;

		case Opcodes::RTI: result = "RTI"; break;

		case Opcodes::RTS: result = "RTS"; break;

		case Opcodes::SBC_immediate: result = "SBC_immediate"; break;
		case Opcodes::SBC_zeropage: result = "SBC_zeropage"; break;
		case Opcodes::SBC_zeropage_X: result = "SBC_zeropage_X"; break;
		case Opcodes::SBC_absolute: result = "SBC_absolute"; break;
		case Opcodes::SBC_absolute_X: result = "SBC_absolute_X"; break;
		case Opcodes::SBC_absolute_Y: result = "SBC_absolute_Y"; break;
		case Opcodes::SBC_indirect_X: result = "SBC_indirect_X"; break;
		case Opcodes::SBC_indirect_Y: result = "SBC_indirect_Y"; break;

		case Opcodes::SEC: result = "SEC"; break;

		case Opcodes::SED: result = "SED"; break;

		case Opcodes::SEI: result = "SEI"; break;

		case Opcodes::STA_zeropage: result = "STA_zeropage"; break;
		case Opcodes::STA_zeropage_X: result = "STA_zeropage_X"; break;
		case Opcodes::STA_absolute: result = "STA_absolute"; break;
		case Opcodes::STA_absolute_X: result = "STA_absolute_X"; break;
		case Opcodes::STA_absolute_Y: result = "STA_absolute_Y"; break;
		case Opcodes::STA_indirect_X: result = "STA_indirect_X"; break;
		case Opcodes::STA_indirect_Y: result = "STA_indirect_Y"; break;

		case Opcodes::STX_zeropage: result = "STX_zeropage"; break;
		case Opcodes::STX_zeropage_Y: result = "STX_zeropage_Y"; break;
		case Opcodes::STX_absolute: result = "STX_absolute"; break;

		case Opcodes::STY_zeropage: result = "STY_zeropage"; break;
		case Opcodes::STY_zeropage_X: result = "STY_zeropage_X"; break;
		case Opcodes::STY_absolute: result = "STY_absolute"; break;
		case Opcodes::TAX: result = "TAX"; break;
		case Opcodes::TAY: result = "TAY"; break;
		case Opcodes::TSX: result = "TSX"; break;
		case Opcodes::TXA: result = "TXA"; break;
		case Opcodes::TXS: result = "TXS"; break;
		case Opcodes::TYA: result = "TYA"; break;
		default: result = "unknown"; break;
	}

	return result;
}

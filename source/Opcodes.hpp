#pragma once

// Based on https://www.masswerk.at/6502/6502_instruction_set.html (Appendix A).
enum class Opcodes
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
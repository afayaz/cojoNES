#include <catch2/catch_test_macros.hpp>

#include <memory>

#include <spdlog/spdlog.h>

#include "CPU.hpp"
#include "Memory.hpp"
#include "System.hpp"
#include "Cartridge.hpp"

std::shared_ptr<CPU>       sCpu;
std::shared_ptr<Memory>    sMemory;
std::shared_ptr<Cartridge> sCart;
std::shared_ptr<System>    sSystem;

void InitSystem()
{
	spdlog::set_level(spdlog::level::trace);

	sCpu = std::make_shared<CPU>();
	sMemory = std::make_shared<Memory>();
	sCart = std::make_shared<Cartridge>();
	sSystem = std::make_shared<System>(sCpu, sMemory, sCart);

	bool loaded = sCart->Load();

	// Write reset vector
	sSystem->Write(0xFFFC, 0x00);
	sSystem->Write(0xFFFD, 0x80);

	sSystem->Reset();
}

void ExecuteSystem()
{
	bool result = true;
	while (result)
	{
		result = sSystem->Process();
	}
}

TEST_CASE("ADC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 10 + 3 = 13.
	REQUIRE(sSystem->Read(0x0000) == 0x0D);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag)    == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag)     == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("AND", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x29); // AND_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 1010 & 0011 = 0010
	REQUIRE(sSystem->Read(0x0000) == 0x02);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag)     == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("ASL", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x0A); // ASL_accumulator
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 01010 << 1 = 10100
	REQUIRE(sSystem->Read(0x0000) == 0x14);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag)    == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag)     == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("BCC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xF0); // literal 240
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x07); // literal 7
	sCart->Write(write_addr++, 0x90); // BCC_relative
	sCart->Write(write_addr++, 0xFC); // literal -3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x05);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
}

TEST_CASE("BCS", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xF0); // literal 240
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x07); // literal 7
	sCart->Write(write_addr++, 0xB0); // BCS_relative
	sCart->Write(write_addr++, 0xFC); // literal -3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0xF7);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == false);
}

TEST_CASE("BEQ", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xF0); // literal 240
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x10); // literal 16
	sCart->Write(write_addr++, 0xF0); // BEQ_relative
	sCart->Write(write_addr++, 0x09); // literal 9
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xB8); // CLV
	sCart->Write(write_addr++, 0x50); // BVC_relative
	sCart->Write(write_addr++, 0x06); // literal 6
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x00);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == true);
}

TEST_CASE("BIT", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xED); // literal 237
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xC4); // literal 196
	sCart->Write(write_addr++, 0x24); // BIT_zeropage
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == true);
}

TEST_CASE("BMI", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xF0); // literal 240
	sCart->Write(write_addr++, 0x30); // BMI_relative
	sCart->Write(write_addr++, 0x04); // literal 4
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x00);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == true);
}

TEST_CASE("BNE", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xD0); // BNE_relative
	sCart->Write(write_addr++, 0x06); // literal 6
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
}

TEST_CASE("BPL", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x10); // BPL_relative
	sCart->Write(write_addr++, 0x06); // literal 6
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("BRK", "[CPU]")
{
	// Currently, nothing to test.
}

TEST_CASE("BVC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xF0); // literal 240
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x50); // BVC_relative
	sCart->Write(write_addr++, 0x06); // literal 6
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0xF0);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
}

TEST_CASE("BVS", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x70); // literal 112
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x13); // literal 19
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x70); // BVS_relative
	sCart->Write(write_addr++, 0x06); // literal 6
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x83);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == true);
}

TEST_CASE("CLC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xFA); // literal 250
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x18); // CLC
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x0000) == 0x04);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
}

TEST_CASE("CLD", "[CPU]")
{
	// This flag is only modified by SED / CLD, and NES doesn't support BCD.
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xD8); // CLD
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_DecimalMode) == false);
}

TEST_CASE("CLI", "[CPU]")
{
	// This flag is only modified by SEI / CLI, so not much to test here...
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0x58); // CLI
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_InterruptDisable) == false);
}

TEST_CASE("CLV", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x70); // literal 112
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x13); // literal 19
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xB8); // CLV
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x83);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
}

TEST_CASE("CMP", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x28); // literal 40
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xCD); // CMP_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("CPX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x28); // literal 40
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xEC); // CPX_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("CPY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x28); // literal 40
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xCC); // CPY_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("DEC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xCE); // DEC_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x29);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("DEX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xCA); // DEX
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x29);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("DEY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x88); // DEY
	sCart->Write(write_addr++, 0x94); // STY_zeropage_X
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x29);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("EOR", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x4D); // EOR_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 0010 1010 XOR 0001 1011 = 0011 0001
	// 42 XOR 27 = 49
	REQUIRE(sSystem->Read(0x00) == 0x31);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("INC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEE); // INC_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2B);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("INX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xE8); // INX
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2B);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("INY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0xC8); // INY
	sCart->Write(write_addr++, 0x94); // STY_zeropage_X
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2B);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("JMP", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	// Store address 0x8017 into 0x0002 and 0x0003 (address of NOP at end of program).
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x17); // literal 23
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x50); // literal 80
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0x4C); // JMP_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("JSR", "[CPU]")
{
	InitSystem();

	// TODO: Come back to this...

	ExecuteSystem();
}

TEST_CASE("LDA", "[CPU]")
{
	InitSystem();

	// Initialise memory to be read from.
	sSystem->Write(0x0000, 0x0C); // 12
	sSystem->Write(0x0001, 0xE0); // 224
	sSystem->Write(0x0002, 0x1B); // 27
	sSystem->Write(0x0003, 0x65); // 101
	sSystem->Write(0x0004, 0xFF); // 255
	sSystem->Write(0x0005, 0x4B); // 75
	sSystem->Write(0x0006, 0x8D); // 141
	sSystem->Write(0x0007, 0x2A); // 42

	// For LDA_indirect_X / LDA_indirect_Y
	sSystem->Write(0x008D, 0x9F); // 159
	sSystem->Write(0x002A, 0xC2); // 194

	uint16_t write_addr = 0x8000;

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x08); // Memory offset 0x08
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA5); // LDA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x09); // Memory offset 0x09

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x02); // literal 2
	sCart->Write(write_addr++, 0xB5); // LDA_zeropage_X
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x0A); // Memory offset 0x0A
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xAD); // LDA_absolute
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x0B); // Memory offset 0x0B
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0xBD); // LDA_absolute_X
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x0C); // Memory offset 0x0C

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x02); // literal 2
	sCart->Write(write_addr++, 0xB9); // LDA_absolute_Y
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x0D); // Memory offset 0x0D
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0xA1); // LDA_indirect_X
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x0E); // Memory offset 0x0E
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x05); // literal 5
	sCart->Write(write_addr++, 0xB1); // LDA_indirect_Y
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x0F); // Memory offset 0x0F
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x08) == 0x2A);
	REQUIRE(sSystem->Read(0x09) == 0x0C);
	REQUIRE(sSystem->Read(0x0A) == 0x1B);
	REQUIRE(sSystem->Read(0x0B) == 0x65);
	REQUIRE(sSystem->Read(0x0C) == 0xFF);
	REQUIRE(sSystem->Read(0x0D) == 0x4B);
	REQUIRE(sSystem->Read(0x0E) == 0x9F);
	REQUIRE(sSystem->Read(0x0F) == 0xC2);
}

TEST_CASE("LDX", "[CPU]")
{
	InitSystem();

	// TODO: Do all of the LDX variants.
	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x65); // literal 101
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA6); // LDX_zeropage
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x86); // STX_zeropage
	sCart->Write(write_addr++, 0x04); // Memory offset 0x04

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sSystem->Read(0x01) == 0xE0);
	REQUIRE(sSystem->Read(0x02) == 0x1B);
	REQUIRE(sSystem->Read(0x03) == 0x65);
	REQUIRE(sSystem->Read(0x04) == 0xE0);
}

TEST_CASE("LDY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x8C); // STY_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0x8C); // STY_absolute
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x8C); // STY_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x65); // literal 101
	sCart->Write(write_addr++, 0x8C); // STY_absolute
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA4); // LDY_zeropage
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x84); // STY_zeropage
	sCart->Write(write_addr++, 0x04); // Memory offset 0x04

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sSystem->Read(0x01) == 0xE0);
	REQUIRE(sSystem->Read(0x02) == 0x1B);
	REQUIRE(sSystem->Read(0x03) == 0x65);
	REQUIRE(sSystem->Read(0x04) == 0xE0);
}

TEST_CASE("LSR", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x4A); // LSR_accumulator
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 01010 >> 1 = 00101
	REQUIRE(sSystem->Read(0x0000) == 0x05);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("NOP", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0xEA); // NOP
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0xEA); // NOP
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 10 + 3 = 13.
	REQUIRE(sSystem->Read(0x0000) == 0x0D);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("ORA", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x09); // ORA_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 1010 | 0011 = 1011
	REQUIRE(sSystem->Read(0x0000) == 0x0B);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("PHA", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x48); // PHA
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x48); // PHA

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x01FD) == 0x0A);
	REQUIRE(sSystem->Read(0x01FC) == 0x2A);
}

TEST_CASE("PHP_PLP", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x08); // PHP
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x00); // literal 00
	sCart->Write(write_addr++, 0x08); // PHP
	sCart->Write(write_addr++, 0x28); // PLP
	sCart->Write(write_addr++, 0x28); // PLP

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sSystem->Read(0x01FD) == 0x00);
	ProcessorStatus status = static_cast<ProcessorStatus>(sSystem->Read(0x01FC));
	REQUIRE((status | PS_ZeroFlag) == PS_ZeroFlag);
}

TEST_CASE("PLA", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x48); // PHA
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x48); // PHA
	sCart->Write(write_addr++, 0x68); // PLA
	sCart->Write(write_addr++, 0x68); // PLA
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x01FD) == 0x0A);
	REQUIRE(sSystem->Read(0x01FC) == 0x2A);
	REQUIRE(sSystem->Read(0x0000) == 0x0A);
}

TEST_CASE("ROL", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xFA); // literal 250
	sCart->Write(write_addr++, 0x2A); // ROL_accumulator
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x0000) == 0xF4);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == true);

}

TEST_CASE("ROR", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xFA); // literal 250
	sCart->Write(write_addr++, 0x6A); // ROR_accumulator
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x0000) == 0x7D);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("RTI", "[CPU]")
{

}

TEST_CASE("RTS", "[CPU]")
{
	// TODO: Implement JSR test first...
}

TEST_CASE("SBC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x38); // SEC
	sCart->Write(write_addr++, 0xE9); // SBC_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	// 10 - 3 = 7.
	REQUIRE(sSystem->Read(0x0000) == 0x07);
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("SEC", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x69); // ADC_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x38); // SEC

	ExecuteSystem();

	// 10 + 3 = 13.
	REQUIRE(sSystem->Read(0x0000) == 0x0D);

	// We set the carry flag ourselves.
	REQUIRE(sCpu->GetProcessorStatus(PS_CarryFlag) == true);
	REQUIRE(sCpu->GetProcessorStatus(PS_ZeroFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_OverflowFlag) == false);
	REQUIRE(sCpu->GetProcessorStatus(PS_NegativeFlag) == false);
}

TEST_CASE("SED", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xF8); // SED

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_DecimalMode) == true);
}

TEST_CASE("SEI", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0x78); // SEI

	ExecuteSystem();

	REQUIRE(sCpu->GetProcessorStatus(PS_InterruptDisable) == true);
}

TEST_CASE("STA", "[CPU]")
{
	// Already in every other test case, if this doesn't work we're screwed.
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x01); // literal 1
	sCart->Write(write_addr++, 0x95); // STA_zeropage_X
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x65); // literal 101
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x02); // literal 2
	sCart->Write(write_addr++, 0x9D); // STA_absolute_X
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x99); // STA_absolute_Y
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0xC2); // literal 194
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x08); // literal 8
	sCart->Write(write_addr++, 0x81); // STA_indirect_X
	sCart->Write(write_addr++, 0x10); // Memory offset 0x10

	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x29); // literal 41
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x07); // literal 7
	sCart->Write(write_addr++, 0x91); // STA_indirect_Y
	sCart->Write(write_addr++, 0x12); // Memory offset 0x12

	// For STA_indirect_X & STA_indirect_Y
	sSystem->Write(0x0018, 0x05);
	sSystem->Write(0x0019, 0x06);

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sSystem->Read(0x01) == 0xE0);
	REQUIRE(sSystem->Read(0x02) == 0x1B);
	REQUIRE(sSystem->Read(0x03) == 0x65);
	REQUIRE(sSystem->Read(0x04) == 0xE0);
	REQUIRE(sSystem->Read(0x05) == 0xC2);
	REQUIRE(sSystem->Read(0x06) == 0x29);
}

TEST_CASE("STX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x86); // STX_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x04); // literal 4
	sCart->Write(write_addr++, 0x96); // STX_zeropage_Y
	sCart->Write(write_addr++, 0x03); // Memory offset 0x03

	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sSystem->Read(0x07) == 0xE0);
	REQUIRE(sSystem->Read(0x02) == 0x1B);
}

TEST_CASE("STY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x2A); // literal 42
	sCart->Write(write_addr++, 0x84); // STY_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0xE0); // literal 224
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x01); // literal 1
	sCart->Write(write_addr++, 0x94); // STY_zeropage_X
	sCart->Write(write_addr++, 0x09); // Memory offset 0x09

	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x1B); // literal 27
	sCart->Write(write_addr++, 0x8C); // STY_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x2A);
	REQUIRE(sSystem->Read(0x0A) == 0xE0);
	REQUIRE(sSystem->Read(0x02) == 0x1B);
}

TEST_CASE("TAX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x45); // literal 69
	sCart->Write(write_addr++, 0xAA); // TAX
	sCart->Write(write_addr++, 0x86); // STX_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x45);
}

TEST_CASE("TAY", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x9E); // literal 158
	sCart->Write(write_addr++, 0xA8); // TAY
	sCart->Write(write_addr++, 0x84); // STY_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x9E);
}

TEST_CASE("TSX", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xBA); // TSX
	sCart->Write(write_addr++, 0x86); // STX_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	// Ideally we'd access the SP but I don't want to make it public.
	REQUIRE(sSystem->Read(0x00) == 0xFD);
}

TEST_CASE("TXA", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x45); // literal 69
	sCart->Write(write_addr++, 0x8A); // TXA
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x45);
}

TEST_CASE("TXS", "[CPU]")
{
	// Not really sure how to verify this since SP is a private member of CPU.
}

TEST_CASE("TYA", "[CPU]")
{
	InitSystem();

	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA0); // LDY_immediate
	sCart->Write(write_addr++, 0x9E); // literal 158
	sCart->Write(write_addr++, 0x98); // TYA
	sCart->Write(write_addr++, 0x85); // STA_zeropage
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x00) == 0x9E);
}

TEST_CASE("Execute Test Program on CPU", "[CPU]")
{
	InitSystem();

	// This is a program to multiply 10 by 3.
	// Taken from part 2 of OLCs NES emulator series
	uint16_t write_addr = 0x8000;
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x0A); // literal 10
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA2); // LDX_immediate
	sCart->Write(write_addr++, 0x03); // literal 3
	sCart->Write(write_addr++, 0x8E); // STX_absolute
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xAC); // LDY_absolute
	sCart->Write(write_addr++, 0x00); // Memory offset 0x00
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xA9); // LDA_immediate
	sCart->Write(write_addr++, 0x00); // literal 0
	sCart->Write(write_addr++, 0x18); // CLC
	sCart->Write(write_addr++, 0x6D); // ADC_absolute
	sCart->Write(write_addr++, 0x01); // Memory offset 0x01
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0x88); // DEY
	sCart->Write(write_addr++, 0xD0); // BNE_relative
	sCart->Write(write_addr++, 0xFA); // literal -5
	sCart->Write(write_addr++, 0x8D); // STA_absolute
	sCart->Write(write_addr++, 0x02); // Memory offset 0x02
	sCart->Write(write_addr++, 0x00); // Memory page 0x00
	sCart->Write(write_addr++, 0xEA); // NOP
	sCart->Write(write_addr++, 0xEA); // NOP
	sCart->Write(write_addr++, 0xEA); // NOP

	ExecuteSystem();

	REQUIRE(sSystem->Read(0x0002) == 0x1E);
}

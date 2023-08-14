#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "CPU.hpp"
#include "Memory.hpp"
#include "System.hpp"
#include "Cartridge.hpp"

TEST_CASE( "Execute Test Program on CPU", "[CPU]" )
{
	std::shared_ptr<CPU>       cpu    = std::make_shared<CPU>();
	std::shared_ptr<Memory>    memory = std::make_shared<Memory>();
	std::shared_ptr<Cartridge> cart   = std::make_shared<Cartridge>();
	std::shared_ptr<System>    system = std::make_shared<System>(cpu, memory, cart);

	cpu->ConnectSystem(system);

	bool loaded = cart->Load();

	// This is a program to multiply 10 by 3.
	// Taken from part 2 of OLCs NES emulator series
	uint16_t write_addr = 0x8000;
	cart->Write(write_addr++, 0xA2); // LDX_immediate
	cart->Write(write_addr++, 0x0A); // literal 10
	cart->Write(write_addr++, 0x8E); // STX_absolute
	cart->Write(write_addr++, 0x00); // Memory offset 0x00
	cart->Write(write_addr++, 0x00); // Memory page 0x00
	cart->Write(write_addr++, 0xA2); // LDX_immediate
	cart->Write(write_addr++, 0x03); // literal 3
	cart->Write(write_addr++, 0x8E); // STX_absolute
	cart->Write(write_addr++, 0x01); // Memory offset 0x01
	cart->Write(write_addr++, 0x00); // Memory page 0x00
	cart->Write(write_addr++, 0xAC); // LDY_absolute
	cart->Write(write_addr++, 0x00); // Memory offset 0x00
	cart->Write(write_addr++, 0x00); // Memory page 0x00
	cart->Write(write_addr++, 0xA9); // LDA_immediate
	cart->Write(write_addr++, 0x00); // literal 0
	cart->Write(write_addr++, 0x18); // CLC
	cart->Write(write_addr++, 0x6D); // ADC_absolute
	cart->Write(write_addr++, 0x01); // Memory offset 0x01
	cart->Write(write_addr++, 0x00); // Memory page 0x00
	cart->Write(write_addr++, 0x88); // DEY
	cart->Write(write_addr++, 0xD0); // BNE_relative
	cart->Write(write_addr++, 0xFA); // literal -5
	cart->Write(write_addr++, 0x8D); // STA_absolute
	cart->Write(write_addr++, 0x02); // Memory offset 0x02
	cart->Write(write_addr++, 0x00); // Memory page 0x00
	cart->Write(write_addr++, 0xEA); // NOP
	cart->Write(write_addr++, 0xEA); // NOP
	cart->Write(write_addr++, 0xEA); // NOP

	// Write reset vector
	system->Write(0xFFFC, 0x00);
	system->Write(0xFFFD, 0x80);

	system->Reset();

	bool result = true;
	while (result)
	{
		result = system->Process();
	}

	REQUIRE(system->Read(0x0002) == 0x1E);
}

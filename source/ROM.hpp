#pragma once

#include <cstdint>
#include <string>
#include <vector>

class ROM
{
public:
	bool Load(const std::string& filename);
	bool Load();

	std::vector<uint8_t>& GetPrgRom();
	std::vector<uint8_t>& GetChrRom();

private:
	std::vector<uint8_t> mPrgRom;
	std::vector<uint8_t> mChrRom;
};

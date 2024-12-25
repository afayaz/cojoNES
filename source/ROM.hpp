#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum HeaderVersion : uint8_t
{
	HV_Unknown,
	HV_iNES_Archaic,
	HV_iNES_0_7,
	HV_iNES_1_0,
	HV_iNES_2_0
};

struct NESHeader
{
	HeaderVersion version;
	size_t prgSize;
	size_t chrSize;
	bool isHMirrored;
	bool hasBattery;
	bool hasTrainer;
	uint16_t mapper;
};

constexpr const char* HeaderVersionToString(HeaderVersion version)
{
	const char* result = "";

	switch (version)
	{
		case HV_iNES_Archaic:
			result = "iNES archaic";
			break;
		case HV_iNES_0_7:
			result = "iNES 0.7";
			break;
		case HV_iNES_1_0:
			result = "iNES 1.0";
			break;
		case HV_iNES_2_0:
			result = "iNES 2.0";
			break;
		case HV_Unknown:
		default:
			result = "Unknown";
			break;
	}

	return result;
}

class ROM
{
public:
	bool Load(const std::string& filename);
	bool Load();

	NESHeader GetHeader() { return mHeader; }

	std::vector<uint8_t>& GetPrgRom();
	std::vector<uint8_t>& GetChrRom();

private:
	NESHeader mHeader;

	std::vector<uint8_t> mPrgRom;
	std::vector<uint8_t> mChrRom;
};

#include "ROM.hpp"

#include <fstream>
#include <vector>

#include <spdlog/spdlog.h>

bool ROM::Load(const std::string& filename)
{
	bool success = false;

	std::ifstream file(filename, std::ios::binary);

	if (file.good())
	{
		file.seekg(0, std::ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		constexpr size_t kHeaderSize = 16;
		constexpr size_t kTrainerSize = 512;

		mHeader.version = HV_Unknown;
		mHeader.prgSize = 0;
		mHeader.chrSize = 0;
		mHeader.isHMirrored = true;
		mHeader.hasBattery = false;
		mHeader.hasTrainer = false;
		mHeader.mapper = 0;

		// Read header
		// Based on info from https://www.nesdev.org/wiki/INES.
		char header[kHeaderSize];
		if (fileSize >= kHeaderSize)
		{
			file.read(header, kHeaderSize);

			success = true;
		}

		if (success)
		{
			if (header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == 0x1A)
			{
				uint8_t headerVersionByte = header[7] & 0x0C;
				if (headerVersionByte == 0x08)
				{
					mHeader.version = HV_iNES_2_0;
				}
				else if (headerVersionByte == 0x04)
				{
					mHeader.version = HV_iNES_Archaic;
				}
				else if (headerVersionByte == 0x00)
				{
					mHeader.version = HV_iNES_1_0;
				}

				if (mHeader.version == HV_iNES_2_0)
				{
					// TODO: For now ignore MSB nibble being 0x0F
					mHeader.prgSize = header[4] | (header[9] & 0x0F) << 8;
					mHeader.prgSize *= 16384;
					mHeader.chrSize = header[5] | (header[9] & 0xF0) << 8;
					mHeader.chrSize *= 8192;
				}
				else
				{
					mHeader.prgSize = header[4] * 16384;
					mHeader.chrSize = header[5] * 8192;
				}

				if ((header[6] & 0x01) == 0x01)
				{
					mHeader.isHMirrored = false;
				}

				if ((header[6] & 0x02) == 0x02)
				{
					mHeader.hasBattery = true;
				}

				if ((header[6] & 0x04) == 0x04)
				{
					mHeader.hasTrainer = true;
				}

				if (mHeader.version == HV_iNES_1_0)
				{
					// Check mapper
					mHeader.mapper = (header[6] & 0xF0) >> 4;
					mHeader.mapper |= (header[7] & 0x0F) << 4;
				}

				std::string headerVersionStr = HeaderVersionToString(mHeader.version);
				SPDLOG_INFO("Successfully read iNES header. Version: {} Trainer: {}, Battery: {} Mapper: {} PRG Size: {} CHR Size: {}", headerVersionStr, mHeader.hasTrainer, mHeader.hasBattery, mHeader.mapper, mHeader.prgSize, mHeader.chrSize);
			}
			else
			{
				success = false;
			}
		}

		// Trainer area
		if (success && mHeader.hasTrainer)
		{
			file.seekg(kTrainerSize, std::ios_base::cur);
		}

		// PRG-ROM
		if (success)
		{
			// TODO: Copying directly to RAM won't work for mappers that use bank switching...
			mPrgRom.resize(static_cast<size_t>(mHeader.prgSize), static_cast<uint8_t>(0));
			file.read(reinterpret_cast<char*>(mPrgRom.data()), mHeader.prgSize);
		}

		// CHR-ROM
		if (success)
		{
			mChrRom.resize(static_cast<size_t>(mHeader.chrSize), static_cast<uint8_t>(0));
			file.read(reinterpret_cast<char*>(mChrRom.data()), mHeader.chrSize);
		}
	}

	return success;
}

bool ROM::Load()
{
	mPrgRom.resize(16384, static_cast<uint8_t>(0));
	mChrRom.resize(16384, static_cast<uint8_t>(0));
	return true;
}

std::vector<uint8_t>& ROM::GetPrgRom()
{
	return mPrgRom;
}

std::vector<uint8_t>& ROM::GetChrRom()
{
	return mChrRom;
}

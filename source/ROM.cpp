#include "ROM.hpp"

#include <fstream>
#include <vector>

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

		bool isNES20 = false;
		bool hasTrainer = false;

		size_t prgSize = 0;
		size_t chrSize = 0;

		// Header
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
				if ((header[7] & 0x0C) == 0x08)
				{
					isNES20 = true;
				}

				if ((header[6] & 0x02) == 0x02)
				{
					hasTrainer = true;
				}

				// TODO: For now ignore MSB nibble being 0x0F
				prgSize = header[4] | (header[9] & 0x0F << 8);
				prgSize *= 16384;
				chrSize = header[5] | (header[9] & 0xF0 << 8);
				chrSize *= 8192;
			}
			else
			{
				success = false;
			}
		}

		// Trainer area
		if (success && hasTrainer)
		{
			file.seekg(kTrainerSize, std::ios_base::cur);
		}

		// PRG-ROM
		if (success)
		{
			// TODO: Copying directly to RAM won't work for mappers that use bank switching...
			mPrgRom.resize(static_cast<size_t>(prgSize), static_cast<uint8_t>(0));
			file.read(reinterpret_cast<char*>(mPrgRom.data()), prgSize);
		}

		// CHR-ROM
		if (success)
		{
			mChrRom.resize(static_cast<size_t>(chrSize), static_cast<uint8_t>(0));
			file.read(reinterpret_cast<char*>(mChrRom.data()), chrSize);
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

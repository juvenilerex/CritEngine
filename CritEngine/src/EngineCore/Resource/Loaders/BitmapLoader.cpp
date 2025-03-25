#include <fstream>

#include "../FileAccessor.h"
#include "BitmapLoader.h"
#include "../../Logging/Logger.h"

namespace Engine {

	enum ColorSpaceEnum : uint8_t
	{
		LCS_CALIBRATED_RGB,
		LCS_GM_ABS_COLORIMETRIC,
		LCS_GM_BUSINESS,
		LCS_GM_GRAPHICS,
		LCS_GM_IMAGES,
		LCS_sRGB,
		LCS_WINDOWS_COLOR_SPACE,
		PROFILE_EMBEDDED,
		PROFILE_LINKED
	};

	struct BitmapFileHeader
	{
		uint16_t signature;
		uint32_t fileSize;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t bitmapOffset;
	};

	struct BitmapCoreHeader
	{
		uint32_t headerSize;
		uint16_t width;
		int16_t height;
		uint16_t planes;
		uint16_t bitCount;
	};

	struct BitmapInfoHeader
	{
		uint32_t headerSize;
		uint32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t imageSize;
		uint32_t pixelsPerMeterX;
		uint32_t pixelsPerMeterY;
		uint32_t colorsUsed;
		uint32_t importantColors;
		uint32_t redBitMask;
		uint32_t greenBitMask;
		uint32_t blueBitMask;
		uint32_t alphaBitMask;
		uint32_t colorSpaceType;
		uint32_t redX;
		uint32_t redY;
		uint32_t redZ;
		uint32_t greenX;
		uint32_t greenY;
		uint32_t greenZ;
		uint32_t blueX;
		uint32_t blueY;
		uint32_t blueZ;
		uint32_t gammaRed;
		uint32_t gammaGreen;
		uint32_t gammaBlue;
		uint32_t intent;
		uint32_t ICCProfileData;
		uint32_t ICCProfileSize;
		uint32_t reserved3;
	};

	enum BitmapHeaderVersion
	{
		BITMAPCOREHEADER = 12,
		BITMAPINFOHEADER = 40,
		BITMAPV2INFOHEADER = 52,
		BITMAPV3INFOHEADER = 56,
		BITMAPV4HEADER = 108,
		BITMAPV5HEADER = 124
	};

	std::string BitmapLoader::GetResourceType()
	{
		return "Image";
	}

	std::shared_ptr<Resource> BitmapLoader::Load(std::filesystem::path filepath)
	{
		std::ifstream file = std::ifstream(filepath, std::ios::binary);

		/*
		FileAccessor ac;

		ac.OpenFile(filepath);
		*/

		if (file.is_open())
		{

			BitmapFileHeader fileHeader{};

			std::byte rawFileHeader[14];
			file.read((char*)rawFileHeader, 14);

			fileHeader.signature = (uint8_t)rawFileHeader[0] | (uint16_t)rawFileHeader[1] << 8;
			fileHeader.fileSize = (uint8_t)rawFileHeader[0] | (uint16_t)rawFileHeader[1] << 8;

			if (fileHeader.signature != 0x4d42)
			{
				LogError("BitmapLoader", "Bitmap file signature is invalid!");
				return nullptr;
			}

			// We incrementally load the header file, once we reach past the read header size, we'll initialize the rest of the data ourselves.

			BitmapInfoHeader bitmapHeader{};
			file.read(reinterpret_cast<char*>(&bitmapHeader), sizeof(4));

			if (!(
				bitmapHeader.headerSize == BITMAPCOREHEADER ||
				bitmapHeader.headerSize == BITMAPINFOHEADER ||
				bitmapHeader.headerSize == BITMAPV2INFOHEADER ||
				bitmapHeader.headerSize == BITMAPV3INFOHEADER ||
				bitmapHeader.headerSize == BITMAPV4HEADER ||
				bitmapHeader.headerSize == BITMAPV5HEADER
				))
			{
				LogError("BitmapLoader", "Bitmap header size is invalid!");
				return nullptr;
			}

			if (bitmapHeader.headerSize == BITMAPCOREHEADER)
			{
				file.seekg(sizeof(BitmapFileHeader));
				file.read(reinterpret_cast<char*>(&bitmapHeader), sizeof(BITMAPCOREHEADER));

				uint32_t width = reinterpret_cast<BitmapCoreHeader*>(&bitmapHeader)->width;
				int32_t height = reinterpret_cast<BitmapCoreHeader*>(&bitmapHeader)->height;

				bitmapHeader.width = width;
				bitmapHeader.height = height;
			}

			if (bitmapHeader.headerSize <= BITMAPINFOHEADER)
			{
				file.seekg(sizeof(BitmapFileHeader));
				file.read(reinterpret_cast<char*>(&bitmapHeader), sizeof(BITMAPINFOHEADER));
			}
			else
			{
				bitmapHeader.compression = 0;
				bitmapHeader.imageSize = 0;
				bitmapHeader.pixelsPerMeterX = 1;
				bitmapHeader.pixelsPerMeterY = 1;
				if (bitmapHeader.bitCount >= 16)
				{
					bitmapHeader.colorsUsed = 0;
				}
				else
				{
					bitmapHeader.colorsUsed = 1 << bitmapHeader.bitCount;
				}
				
				bitmapHeader.importantColors = 0;
			}

			if (bitmapHeader.headerSize <= BITMAPV2INFOHEADER)
			{
				file.read(reinterpret_cast<char*>(&bitmapHeader + BITMAPINFOHEADER), sizeof(BITMAPV2INFOHEADER - BITMAPINFOHEADER));
			}
			else
			{
				if (bitmapHeader.bitCount == 16)
				{
					bitmapHeader.redBitMask =   0b11111000000000000000000000000000;
					bitmapHeader.greenBitMask = 0b00000111111000000000000000000000;
					bitmapHeader.blueBitMask =  0b00000000000111110000000000000000;
				}
				else if (bitmapHeader.bitCount == 24)
				{
					bitmapHeader.redBitMask =   0b11111111000000000000000000000000;
					bitmapHeader.greenBitMask = 0b00000000111111110000000000000000;
					bitmapHeader.blueBitMask =  0b00000000000000001111111100000000;
				}
			}

			if (bitmapHeader.headerSize <= BITMAPV3INFOHEADER)
			{
				file.read(reinterpret_cast<char*>(&bitmapHeader + BITMAPV2INFOHEADER), sizeof(BITMAPV3INFOHEADER - BITMAPV2INFOHEADER));
			}
			else
			{
				bitmapHeader.alphaBitMask = 0b00000000000000000000000000000000;
			}

			if (bitmapHeader.headerSize <= BITMAPV4HEADER)
			{
				file.read(reinterpret_cast<char*>(&bitmapHeader + BITMAPV3INFOHEADER), sizeof(BITMAPV4HEADER - BITMAPV3INFOHEADER));
			}
			else
			{
				bitmapHeader.colorSpaceType = 1;
				bitmapHeader.redX = 0;
				bitmapHeader.redY = 0;
				bitmapHeader.redZ = 0;
				bitmapHeader.greenX = 0;
				bitmapHeader.greenY = 0;
				bitmapHeader.greenZ = 0;
				bitmapHeader.blueX = 0;
				bitmapHeader.blueY = 0;
				bitmapHeader.blueZ = 0;
				bitmapHeader.gammaRed = 1;
				bitmapHeader.gammaGreen = 1;
				bitmapHeader.gammaBlue = 1;
			}

			if (bitmapHeader.headerSize == BITMAPV5HEADER)
			{
				file.read(reinterpret_cast<char*>(&bitmapHeader + BITMAPV4HEADER), sizeof(BITMAPV5HEADER - BITMAPV4HEADER));
			}
			else
			{
				bitmapHeader.intent = LCS_CALIBRATED_RGB;
				bitmapHeader.ICCProfileData = 0;
				bitmapHeader.ICCProfileSize = 0;
				bitmapHeader.reserved3 = 0;
			}
			
			// Process Data.
			std::vector<char> image = std::vector<char>();
			image.reserve(bitmapHeader.width * std::abs(bitmapHeader.height) * 4);
			std::vector<char> rawRow = std::vector<char>();
			rawRow.reserve(bitmapHeader.width* bitmapHeader.bitCount / 8);

			file.seekg(fileHeader.bitmapOffset);

			float uint32RowSize = (float)(bitmapHeader.width * bitmapHeader.bitCount) / sizeof(uint32_t);
			uint8_t rowPadding = (uint8_t)(std::ceil(uint32RowSize) - uint32RowSize) * sizeof(uint32_t);

			int redBitMaskOffset = 0;
			int greenBitMaskOffset = 0;
			int blueBitMaskOffset = 0;
			int alphaBitMaskOffset = 0;
			for (int i = 0; i < 31; i++)
			{
				if (((1 << i) & bitmapHeader.redBitMask >> i) == 1) redBitMaskOffset = i;
				if (((1 << i) & bitmapHeader.greenBitMask >> i) == 1) greenBitMaskOffset = i;
				if (((1 << i) & bitmapHeader.blueBitMask >> i) == 1) blueBitMaskOffset = i;
				if (((1 << i) & bitmapHeader.alphaBitMask >> i) == 1) alphaBitMaskOffset = i;
			}
			
			for (uint32_t y = 0; y < (uint32_t) abs(bitmapHeader.height); y++)
			{
				file.read(rawRow.data(), bitmapHeader.width * bitmapHeader.bitCount / 8);
				file.seekg(rowPadding, std::ios::cur);

				for (uint32_t x = 0; x < bitmapHeader.width; x++)
				{
					if (bitmapHeader.bitCount == 1)
					{
						// Get Color Palette Data and parse it into RGBA8888 format
					}
					else if (bitmapHeader.bitCount == 4)
					{
						// Get Color Palette Data and parse it into RGBA8888 format
					}
					else if (bitmapHeader.bitCount == 8)
					{
						// Get Color Palette Data and parse it into RGBA8888 format
					}
					else if (bitmapHeader.bitCount == 16)
					{
						uint16_t color = 
							rawRow[x * bitmapHeader.bitCount / 8] << 8 | 
							rawRow[x * bitmapHeader.bitCount / 8 + 1];

						image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 3] = 0;
					}
					else if (bitmapHeader.bitCount == 24)
					{
						image[(x + bitmapHeader.width * y) * 4] = rawRow[x * bitmapHeader.bitCount / 8];
						image[(x + bitmapHeader.width * y) * 4 + 1] = rawRow[x * bitmapHeader.bitCount / 8 + 1];
						image[(x + bitmapHeader.width * y) * 4 + 2] = rawRow[x * bitmapHeader.bitCount / 8 + 2];
						image[(x + bitmapHeader.width * y) * 4 + 3] = (uint8_t)255;
					}
					else if (bitmapHeader.bitCount == 32)
					{
						uint32_t color =
							rawRow[x * bitmapHeader.bitCount / 8] << 24 |
							rawRow[x * bitmapHeader.bitCount / 8 + 1] << 16 |
							rawRow[x * bitmapHeader.bitCount / 8 + 2] << 8 |
							rawRow[x * bitmapHeader.bitCount / 8 + 3];

						image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 3] = (color & bitmapHeader.alphaBitMask) >> alphaBitMaskOffset;
					}
				}
			}
			LogInfo("BitmapLoader", "Texture stored");
			return std::make_shared<Image>(image.data(), bitmapHeader.width, bitmapHeader.height, 4);


		}

		LogError("BitmapLoader", "Failed to open file.");
		return nullptr;
	}
}
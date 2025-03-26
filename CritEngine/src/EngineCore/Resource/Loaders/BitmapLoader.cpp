#include <fstream>
#include <array>

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
		int16_t width;
		int16_t height;
		uint16_t planes;
		uint16_t bitCount;
	};

	struct BitmapInfoHeader
	{
		uint32_t headerSize;
		int32_t width;
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
		FileAccessor file = FileAccessor();
		file.OpenFile(filepath);

		if (file.GetSize() <= 0)
		{
			LogError("BitmapLoader", "Failed to open file.");
			return nullptr;
		};

		BitmapFileHeader fileHeader;
		fileHeader.signature = file.ReadUINT16();
		fileHeader.fileSize = file.ReadUINT32();
		fileHeader.reserved1 = file.ReadUINT16();
		fileHeader.reserved2 = file.ReadUINT16();
		fileHeader.bitmapOffset = file.ReadUINT32();

		if (fileHeader.signature != 0x4d42)
		{
			LogError("BitmapLoader", "Bitmap file signature is invalid!");
			return nullptr;
		}

		// We incrementally load the header file, once we reach past the read header size, we'll initialize the rest of the data ourselves.

		BitmapInfoHeader bitmapHeader;
		bitmapHeader.headerSize = file.ReadUINT32();

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
			bitmapHeader.width = file.ReadINT16();
			bitmapHeader.height = file.ReadINT16();
			bitmapHeader.planes = file.ReadUINT16();
			bitmapHeader.bitCount = file.ReadUINT16();
		}

		if (bitmapHeader.headerSize >= BITMAPINFOHEADER)
		{
			bitmapHeader.width = file.ReadINT32();
			bitmapHeader.height = file.ReadINT32();
			bitmapHeader.planes = file.ReadUINT16();
			bitmapHeader.bitCount = file.ReadUINT16();
			bitmapHeader.compression = file.ReadUINT32();
			bitmapHeader.imageSize = file.ReadUINT32();
			bitmapHeader.pixelsPerMeterX = file.ReadUINT32();
			bitmapHeader.pixelsPerMeterY = file.ReadUINT32();
			bitmapHeader.colorsUsed = file.ReadUINT32();
			bitmapHeader.importantColors = file.ReadUINT32();
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

		if(!(
			bitmapHeader.bitCount == 1 ||
			bitmapHeader.bitCount == 4 ||
			bitmapHeader.bitCount == 8 ||
			bitmapHeader.bitCount == 16 ||
			bitmapHeader.bitCount == 24 ||
			bitmapHeader.bitCount == 32
			))
		{
			LogError("BitmapLoader", "Bits per pixel is invalid, must be 1, 4, 8, 16, 24, or 32.");
			return nullptr;
		}

		if (bitmapHeader.headerSize >= BITMAPV2INFOHEADER)
		{
			bitmapHeader.redBitMask = file.ReadUINT32();
			bitmapHeader.greenBitMask = file.ReadUINT32();
			bitmapHeader.blueBitMask = file.ReadUINT32();
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
				bitmapHeader.redBitMask =   0b00000000000000000000000000000000;
				bitmapHeader.greenBitMask = 0b00000000000000000000000000000000;
				bitmapHeader.blueBitMask =  0b00000000000000000000000000000000;
			}
		}

		if (bitmapHeader.headerSize >= BITMAPV3INFOHEADER)
		{
			bitmapHeader.alphaBitMask = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.alphaBitMask = 0b00000000000000000000000000000000;
		}

		if (bitmapHeader.headerSize >= BITMAPV4HEADER)
		{
			bitmapHeader.colorSpaceType = file.ReadUINT32();
			bitmapHeader.redX = file.ReadUINT32();
			bitmapHeader.redY = file.ReadUINT32();
			bitmapHeader.redZ = file.ReadUINT32();
			bitmapHeader.greenX = file.ReadUINT32();
			bitmapHeader.greenY = file.ReadUINT32();
			bitmapHeader.greenZ = file.ReadUINT32();
			bitmapHeader.blueX = file.ReadUINT32();
			bitmapHeader.blueY = file.ReadUINT32();
			bitmapHeader.blueZ = file.ReadUINT32();
			bitmapHeader.gammaRed = file.ReadUINT32();
			bitmapHeader.gammaGreen = file.ReadUINT32();
			bitmapHeader.gammaBlue = file.ReadUINT32();
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
			bitmapHeader.intent = file.ReadUINT32();
			bitmapHeader.ICCProfileData = file.ReadUINT32();
			bitmapHeader.ICCProfileSize = file.ReadUINT32();
			bitmapHeader.reserved3 = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.intent = LCS_CALIBRATED_RGB;
			bitmapHeader.ICCProfileData = 0;
			bitmapHeader.ICCProfileSize = 0;
			bitmapHeader.reserved3 = 0;
		}
		
		// Prepare for processing color data.
		std::vector<char> image = std::vector<char>();
		image.resize(std::abs(bitmapHeader.width) * std::abs(bitmapHeader.height) * 4);
		std::array<char, 4> rawColorData = { 0, 0, 0, 0 };

		float uint32RowSize = ((float)bitmapHeader.width * ((float)bitmapHeader.bitCount / 8)) / sizeof(uint32_t);
		uint8_t rowPadding = (uint8_t)(std::ceil(uint32RowSize) - uint32RowSize) * sizeof(uint32_t);

		uint8_t redBitMaskOffset = 0;
		uint8_t greenBitMaskOffset = 0;
		uint8_t blueBitMaskOffset = 0;
		uint8_t alphaBitMaskOffset = 0;
		bool useDefaultBitMask = false;
		for (int i = 0; i < 31; i++)
		{
			if (((1 << i) & bitmapHeader.redBitMask >> i) == 1) redBitMaskOffset = i;
			if (((1 << i) & bitmapHeader.greenBitMask >> i) == 1) greenBitMaskOffset = i;
			if (((1 << i) & bitmapHeader.blueBitMask >> i) == 1) blueBitMaskOffset = i;
			if (((1 << i) & bitmapHeader.alphaBitMask >> i) == 1) alphaBitMaskOffset = i;
		}
		useDefaultBitMask = (redBitMaskOffset | greenBitMaskOffset | blueBitMaskOffset | alphaBitMaskOffset) == 0;
		
		// Process color Data.
		file.Seek(fileHeader.bitmapOffset);

		for (uint32_t y = 0; y < (uint32_t) abs(bitmapHeader.height); y++)
		{
			for (uint32_t x = 0; x < (uint32_t) abs(bitmapHeader.width); x++)
			{
				if (std::fmod(x % 8, 1.f / ((float)bitmapHeader.bitCount / 8.f)) < 1)
				{
					std::vector<uint8_t> vec = file.ReadBuffer(bitmapHeader.bitCount / 8);
					std::copy(vec.begin(), vec.end(), rawColorData.begin());
				}

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
						rawColorData[0] << 8 |
						rawColorData[1];

					image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
					image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
					image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
					image[(x + bitmapHeader.width * y) * 4 + 3] = 0;
				}
				else if (bitmapHeader.bitCount == 24)
				{
					if (useDefaultBitMask)
					{
						image[(x + bitmapHeader.width * y) * 4] = rawColorData[2];
						image[(x + bitmapHeader.width * y) * 4 + 1] = rawColorData[1];
						image[(x + bitmapHeader.width * y) * 4 + 2] = rawColorData[0];
						image[(x + bitmapHeader.width * y) * 4 + 3] = (uint8_t)0xFF;
					}
					else
					{
						uint32_t color =
							rawColorData[0] << 16 |
							rawColorData[1] << 8 |
							rawColorData[2];

						image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 3] = 0;
					}
				}
				else if (bitmapHeader.bitCount == 32)
				{
					if (useDefaultBitMask)
					{
						image[(x + bitmapHeader.width * y) * 4] = rawColorData[2];
						image[(x + bitmapHeader.width * y) * 4 + 1] = rawColorData[1];
						image[(x + bitmapHeader.width * y) * 4 + 2] = rawColorData[0];
						image[(x + bitmapHeader.width * y) * 4 + 3] = rawColorData[3];
					}
					else
					{
						uint32_t color =
							rawColorData[0] << 24 |
							rawColorData[1] << 16 |
							rawColorData[2] << 8 |
							rawColorData[3];

						image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
						image[(x + bitmapHeader.width * y) * 4 + 3] = (color & bitmapHeader.alphaBitMask) >> alphaBitMaskOffset;
					}
				}
			}

			file.Seek(file.GetPosition() + rowPadding);
		}
		LogInfo("BitmapLoader", "Texture stored");
		return std::make_shared<Image>(image.data(), abs(bitmapHeader.width), abs(bitmapHeader.height), 4);
	}
}
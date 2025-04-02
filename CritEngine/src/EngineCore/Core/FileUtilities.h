#pragma once

#include <cstdint>

namespace Engine {
	static inline uint16_t BSWAP16(uint16_t x)
	{
		return (x >> 8) | (x << 8);
	}

	static inline uint32_t BSWAP32(uint32_t x)
	{
		return ((x << 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x >> 24));
	}

	static inline uint64_t BSWAP64(uint64_t x)
	{
		x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
		x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
		x = (x & 0x00FF00FF00FF00FF) << 8 | (x & 0xFF00FF00FF00FF00) >> 8;
		return x;
	}
}
#pragma once

#include <memory>

#include "../Resource.h"
#include "../../Graphics/Texture.h"

namespace Engine {
	class Image : public Resource
	{
	public:

		Image();
		Image(char* data, uint32_t width, uint32_t height, uint8_t channelCount);

		Texture* GetTexture();

		bool IsValid();

	private:
		std::unique_ptr<Texture> texture;
	};
}


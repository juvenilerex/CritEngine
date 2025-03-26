#pragma once

#include <memory>

#include "../Resource.h"
#include "../../Graphics/Texture.h"

namespace Engine {
	class Image : public Resource
	{
	public:

		ENGINE_API Image();
		ENGINE_API Image(char* data, uint32_t width, uint32_t height, uint8_t channelCount);

		ENGINE_API virtual ~Image() override;

		ENGINE_API Texture* GetTexture();

		ENGINE_API bool IsValid();

	private:
		std::unique_ptr<Texture> texture;
	};
}


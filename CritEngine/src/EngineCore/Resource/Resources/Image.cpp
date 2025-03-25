#include "Image.h"

namespace Engine {

	Image::Image()
	{

	};

	Image::Image(char* data, uint32_t width, uint32_t height, uint8_t channelCount)
		: texture(Texture::CreateUnique(data, width, height, channelCount))
	{
	};

	Image::~Image()
	{
	};

	Texture* Image::GetTexture()
	{
		return this->texture.get();
	}

	bool Image::IsValid()
	{
		return this->texture != nullptr;
	}
}
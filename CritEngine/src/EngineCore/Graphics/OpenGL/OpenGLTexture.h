#pragma once

#include "../Texture.h"

namespace Engine {
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(char* vertices, uint32_t width, uint32_t height, uint8_t channelCount);
		virtual ~OpenGLTexture();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t textureID;
	};
}
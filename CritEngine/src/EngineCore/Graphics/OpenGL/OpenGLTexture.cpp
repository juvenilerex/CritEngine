#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Engine {

	OpenGLTexture::OpenGLTexture(char* data, uint32_t width, uint32_t height, uint8_t channelCount)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &this->textureID);
		glTexImage2D(this->textureID, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &this->textureID);
	}

	void OpenGLTexture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, this->textureID);
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}
#pragma once

#include "../Core/Base.h"

namespace Engine {

	class Texture
	{
	public:
		virtual ~Texture() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		ENGINE_API static std::shared_ptr<Texture> Create(char* vertices, uint32_t width, uint32_t height, uint8_t channelCount);
	};

}
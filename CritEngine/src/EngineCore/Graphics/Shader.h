#pragma once
#include <string>
#include "../Logging/Logger.h"

namespace Engine {
	class Shader
	{
	public:
		ENGINE_API Shader(const std::string& vertexSource, std::string& fragmentSource);
		ENGINE_API ~Shader();

		ENGINE_API void Bind() const;
		ENGINE_API void Unbind() const;
	private:
		uint32_t shaderID;
	};

}
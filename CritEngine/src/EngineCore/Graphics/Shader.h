#pragma once
#include <string>
#include "../Logging/Logger.h"

namespace Engine {
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t shaderID;
	};

}
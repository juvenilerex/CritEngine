#pragma once
#include <string>
#include "../Logging/Logger.h"
#include "../Math/Common.h"

namespace Engine {
	class Shader
	{
	public:
		ENGINE_API Shader(const std::string& vertexSource, std::string& fragmentSource);
		ENGINE_API ~Shader();

		ENGINE_API void Bind() const;
		ENGINE_API void Unbind() const;

		ENGINE_API void UploadUniformMat4(const std::string name, const Matrix4f& mat4);
	private:
		uint32_t shaderID;
	};

}
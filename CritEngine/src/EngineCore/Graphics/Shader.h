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

		ENGINE_API void UploadUniformMat3(const std::string name, const Matrix3f& mat3);
		ENGINE_API void UploadUniformMat4(const std::string name, const Matrix4f& mat4);

		ENGINE_API void UploadUniformFloat(const std::string name, const float& float1);
		ENGINE_API void UploadUniformFloat2(const std::string name, const Vector2& float2);
		ENGINE_API void UploadUniformFloat3(const std::string name, const Vector3& float3);
		ENGINE_API void UploadUniformFloat4(const std::string name, const Vector4& float4);

		ENGINE_API void UploadUniformInt(const std::string name, const int& int1);

		ENGINE_API void UploadUniformBool(const std::string name, const int& boolean);
	private:
		uint32_t shaderID;
	};

}
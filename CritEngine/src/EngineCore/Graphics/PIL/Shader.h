#pragma once
#include <string>
#include "../../Core/Base.h"
#include "../../Math/Common.h"


namespace Engine {
	class Shader
	{
	public:
		ENGINE_API static std::shared_ptr<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);

		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMat3(const std::string name, const Matrix3f& mat3) = 0;
		virtual void UploadUniformMat4(const std::string name, const Matrix4f& mat4) = 0;
		virtual void UploadUniformFloat(const std::string name, const float& float1) = 0;
		virtual void UploadUniformFloat2(const std::string name, const Vector2& float2) = 0;
		virtual void UploadUniformFloat3(const std::string name, const Vector3& float3) = 0;
		virtual void UploadUniformFloat4(const std::string name, const Vector4& float4) = 0;
		virtual void UploadUniformInt(const std::string name, const int& int1) = 0;
		virtual void UploadUniformBool(const std::string name, const int& boolean) = 0;
	};

}
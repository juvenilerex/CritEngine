#pragma once
#include "../Shader.h"

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& glslSource, const ShaderType shaderType);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformMat3(const std::string name, const Matrix3f& mat3) override;
		virtual void UploadUniformMat4(const std::string name, const Matrix4f& mat4) override;
		virtual void UploadUniformFloat(const std::string name, const float& float1) override;
		virtual void UploadUniformFloat2(const std::string name, const Vector2& float2) override;
		virtual void UploadUniformFloat3(const std::string name, const Vector3& float3) override;
		virtual void UploadUniformFloat4(const std::string name, const Vector4& float4) override;
		virtual void UploadUniformInt(const std::string name, const int& int1) override;
		virtual void UploadUniformBool(const std::string name, const int& boolean) override;

	private:
		uint32_t shaderID;
	};

}
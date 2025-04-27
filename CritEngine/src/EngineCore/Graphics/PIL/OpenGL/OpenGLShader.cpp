#pragma once

#include <glad/glad.h>
#include <vector>
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"


namespace Engine {

	uint32_t EngineShaderTypeToOpenGLShaderType(ShaderType shaderType)
	{
		switch (shaderType)
		{
			case ShaderType::Vertex: return GL_VERTEX_SHADER;
			case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
			case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
			case ShaderType::TessellationControl: return GL_TESS_CONTROL_SHADER;
			case ShaderType::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
			case ShaderType::Compute: return GL_COMPUTE_SHADER;
			default: return GL_NONE;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& glslSource, const ShaderType shaderType)
		: shaderID(0)
	{
		ASSERT(shaderType != ShaderType::Task && shaderType != ShaderType::Mesh, "OpenGL doesn't support Task & Mesh Shaders!");

		uint32_t shader = glCreateShader(EngineShaderTypeToOpenGLShaderType(shaderType));
		const char* source = glslSource.c_str();
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		int32_t isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			int32_t maxMessageLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetShaderInfoLog(shader, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteShader(shader);

			LogError("OpenGL", "Shader Compilation Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));

			return;
		}

		// TODO: This functionality is best suited for an OpenGL pipeline object.
		/*
		uint32_t program = glCreateProgram();
		this->shaderID = program;

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		int32_t isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == false)
		{
			int32_t maxMessageLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetProgramInfoLog(program, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			LogError("OpenGL", "Shader Link Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));
			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		*/
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(this->shaderID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(this->shaderID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat3(const std::string name, const Matrix3f& mat3)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat3.data[0]);
	}
	void OpenGLShader::UploadUniformMat4(const std::string name, const Matrix4f& mat4)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat4.data[0]);
	}
	void OpenGLShader::UploadUniformFloat(const std::string name, const float& float1)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform1fv(location, 1, &float1);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string name, const Vector2& float2)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform2fv(location, 1, &float2.x);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string name, const Vector3& float3)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform3fv(location, 1, &float3.x);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string name, const Vector4& float4)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform4fv(location, 1, &float4.x);
	}
	void OpenGLShader::UploadUniformInt(const std::string name, const int& int1)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform1iv(location, 1, &int1);
	}
	void OpenGLShader::UploadUniformBool(const std::string name, const int& boolean)
	{
		int location = glGetUniformLocation(this->shaderID, name.c_str());
		glUniform1iv(location, 1, &boolean);
	}
}
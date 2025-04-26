#pragma once

#include <glad/glad.h>
#include <vector>
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"


namespace Engine {
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
		: shaderID(0)
	{
		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		int32_t isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			int32_t maxMessageLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetShaderInfoLog(vertexShader, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			LogError("OpenGL", "Vertex Shader Compilation Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));

			return;
		}

		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			int32_t maxMessageLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetShaderInfoLog(fragmentShader, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			LogError("OpenGL", "Fragment Shader Compilation Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));

			return;
		}

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
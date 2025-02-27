#pragma once

#include <glad/glad.h>
#include <vector>
#include "../Shader.h"


namespace Engine {
	Shader::Shader(const std::string& vertexSource, std::string& fragmentSource)
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
	Shader::~Shader()
	{
		glDeleteProgram(this->shaderID);
	}
	void Shader::Bind() const
	{
		glUseProgram(this->shaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}
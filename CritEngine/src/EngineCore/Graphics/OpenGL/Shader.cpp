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

		this->rendererID = glCreateProgram();

		glAttachShader(this->rendererID, vertexShader);
		glAttachShader(this->rendererID, fragmentShader);

		glLinkProgram(this->rendererID);

		int32_t isLinked = 0;
		glGetProgramiv(this->rendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == false)
		{
			int32_t maxMessageLength = 0;
			glGetProgramiv(this->rendererID, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetProgramInfoLog(this->rendererID, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteProgram(this->rendererID);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			LogError("OpenGL", "Shader Link Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));
			return;
		}

		glDetachShader(this->rendererID, vertexShader);
		glDetachShader(this->rendererID, fragmentShader);

	}
	Shader::~Shader()
	{
		glDeleteProgram(this->rendererID);
	}
	void Shader::Bind() const
	{
		glUseProgram(this->rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}
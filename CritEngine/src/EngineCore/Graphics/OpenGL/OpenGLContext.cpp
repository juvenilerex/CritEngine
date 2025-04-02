#pragma once
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: windowHandle(windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(this->windowHandle);
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success, "Failed to initialize GLAD!");
	}

	void  OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(this->windowHandle);
	}
}
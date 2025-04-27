#include "OpenGLGraphicsEntry.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	void OpenGLGraphicsEntry::Init()
	{
		glfwMakeContextCurrent(this->windowHandle);
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success, "Failed to initialize GLAD!");
	}

}
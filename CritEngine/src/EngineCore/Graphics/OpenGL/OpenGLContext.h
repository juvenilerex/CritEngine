#pragma once

#include "../RenderContext.h"
#include "../../Window/Window.h"

struct GLFWwindow;

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}
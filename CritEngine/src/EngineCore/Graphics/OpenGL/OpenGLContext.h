#pragma once

#include "../RenderContext.h"
#include "../../Window/Window.h"

struct GLFWwindow;

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void InitImGui() override;
		virtual void ImGuiStartFrame() override;
		virtual void ImGuiRender() override;

	private:
		GLFWwindow* windowHandle;
	};
}
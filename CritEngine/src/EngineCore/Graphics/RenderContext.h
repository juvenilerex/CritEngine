#pragma once

#ifdef CE_RENDERER_OPENGL4
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#endif

namespace Engine {

	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void InitImGui() = 0;
		virtual void ImGuiStartFrame() = 0;
		virtual void ImGuiRender() = 0;
	};
}
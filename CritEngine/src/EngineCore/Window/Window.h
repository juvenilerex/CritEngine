#pragma once

#include "../Event/Event.h"
#include "../Logging/Logger.h"
#include "../Graphics/RenderContext.h"
#include "../Graphics/Renderer.h"
#include "Input.h"
#include "InputMouse.h"
#include <string>

struct GLFWwindow;

namespace Engine {

	class Window
	{
	public:

		ENGINE_API Window(int width, int height, std::string title);
		ENGINE_API ~Window();

		ENGINE_API GLFWwindow* GetHandle();

		ENGINE_API void PollEvents();
		ENGINE_API void SwapBuffers();

		ENGINE_API MouseInputListener& GetMouseInput();
		ENGINE_API InputListener& GetInput();

		ENGINE_API int GetWidth() { return this->width; }
		ENGINE_API int GetHeight() { return this->height; }
		ENGINE_API float GetAspectRatio() { return (float)this->width / (float)this->height; }

	private:
		GLFWwindow* windowHandle = nullptr;
		std::unique_ptr<MouseInputListener> mouseInput = nullptr;
		std::unique_ptr<InputListener> input = nullptr;
		std::unique_ptr<RenderContext> renderContext = nullptr;
		int width, height;

	};

};
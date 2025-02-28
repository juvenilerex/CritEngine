#pragma once

#include "../Event/Event.h"
#include "../Logging/Logger.h"
#include "../Graphics/RenderContext.h"
#include "../Graphics/Graphics.h"
#include "InputMouse.h"
#include "Input.h"
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

		ENGINE_API MousePositionListener& GetMousePosition();
		ENGINE_API MouseInputListener& GetMouseButtonInput();
		ENGINE_API InputListener& GetInput();
		ENGINE_API GraphicsRenderer& GetRenderer();

	private:
		GLFWwindow* windowHandle = nullptr;
		std::unique_ptr<MousePositionListener> mousePos = nullptr;
		std::unique_ptr<MouseInputListener> mouseInput = nullptr;
		std::unique_ptr<InputListener> input = nullptr;
		std::unique_ptr<RenderContext> renderContext = nullptr;
		std::unique_ptr<GraphicsRenderer> renderer = nullptr;

	};

};
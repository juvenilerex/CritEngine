#pragma once

#include <memory>

#include "Core.h"
#include "Graphics.h"
#include "Window.h"
#include "Input.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		~Application();

		void TickInternal();
		void virtual Tick();
		bool GetKeyJustPressed(int key);
		bool GetKeyDown(int key);
		bool GetKeyReleased(int key);

	private:
		std::unique_ptr<Window> window = nullptr;
		std::unique_ptr<GraphicsRenderer> renderer = nullptr;
		std::unique_ptr<InputListener> input = nullptr;
	};

	std::unique_ptr<Application> CreateApplication();
}




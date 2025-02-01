#pragma once

#include <memory>

#include "Core.h"
#include "Graphics.h"
#include "Window.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		~Application();

		void TickInternal();
		void virtual Tick();

	private:
		std::unique_ptr<Window> window = nullptr;
		std::unique_ptr<GraphicsRenderer> renderer = nullptr;
	};

	std::unique_ptr<Application> CreateApplication();
}




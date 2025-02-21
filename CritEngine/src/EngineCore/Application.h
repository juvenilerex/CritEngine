#pragma once

#include <memory>


#include "Core/Base.h"
#include "Window/Window.h"
#include "LayerStack.h"

namespace Engine {

	class Application
	{
	public:

		ENGINE_API Application();
		ENGINE_API ~Application();

		ENGINE_API void TickInternal();
		ENGINE_API void virtual Tick();

		ENGINE_API void PushLayer(Layer* layer);
		ENGINE_API void PushOverlay(Layer* overlay);

		ENGINE_API Window& GetWindow();

	private:
		std::unique_ptr<Window> window = nullptr;
		LayerStack layerStack;

	};

	std::unique_ptr<Application> CreateApplication();
}




#pragma once

#include "Core/Base.h"
#include "Window/Window.h"
#include "LayerStack.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Input/Input.h"

namespace Engine {

	class Application
	{
	public:

		ENGINE_API Application();
		ENGINE_API ~Application();

		ENGINE_API void TickInternal();
		ENGINE_API void virtual Tick();
		ENGINE_API void virtual OnInputEvent(Event& event);

		ENGINE_API void PushLayer(Layer* layer);
		ENGINE_API void PushOverlay(Layer* overlay);

		ENGINE_API void OnEvent(Event& event);

		ENGINE_API Window& GetWindow();
		ENGINE_API InputListener& GetInput() const;

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnWindowClose(WindowCloseEvent& event);

	private:					
		std::unique_ptr<Window> window = nullptr;
		LayerStack layerStack;
	};

	std::unique_ptr<Application> CreateApplication();
}




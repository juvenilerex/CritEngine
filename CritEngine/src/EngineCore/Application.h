#pragma once

#include "Core/Base.h"
#include "Window/Window.h"
#include "LayerStack.h"
#include "Event/WindowEvent.h"

#define BIND_EVENT_FUNC(func) [this] (auto& event) -> decltype(auto) { return func(event); }

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

		ENGINE_API void OnEvent(Event& event);

		ENGINE_API Window& GetWindow();
		ENGINE_API InputListener& GetInput() const;
		ENGINE_API MouseInputListener& GetMouseInput() const;

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnWindowClose(WindowCloseEvent& event);

	private:					
		std::unique_ptr<Window> window = nullptr;
		LayerStack layerStack;
	};

	std::unique_ptr<Application> CreateApplication();
}




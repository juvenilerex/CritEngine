#include "Application.h"
#include "Logging/Logger.h"
#include "Event/Event.h"

namespace Engine {

	Application::Application()
	{
		this->window = std::make_unique<Window>(800, 600, "Sandbox");
	};

	Application::~Application()
	{
		this->window.reset();
	};

	void Application::TickInternal()
	{
		if (this->window != nullptr)
		{
			for (Layer* layer : this->layerStack)
				layer->OnUpdate();
			this->window->PollEvents();
			this->window->SwapBuffers();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		this->layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		this->layerStack.PushLayer(overlay);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		for (auto it = layerStack.begin(); it != layerStack.end(); it++) {
			if (event.IsHandled())
				return;
			(*it)->OnEvent(event);
		}
	}

	Window& Application::GetWindow()
	{
		ASSERT(this->window)
		return *this->window.get();
	}

	InputListener& Application::GetInput() const
	{
		return this->window->GetInput();
	}

	MouseInputListener& Application::GetMouseInput() const
	{
		return this->window->GetMouseInput();
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}


#include "Application.h"
#include "Logging/Logger.h"

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

	Window& Application::GetWindow()
	{
		ASSERT(this->window)
		return *this->window.get();
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}


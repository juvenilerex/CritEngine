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
			this->window->GetRenderer().Draw();
			this->window->SwapBuffers();
		}
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


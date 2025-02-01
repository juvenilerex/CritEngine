#include "Application.h"
#include "Logging/Logger.h"


namespace Engine {

	Application::Application()
	{
		this->window = std::make_unique<Engine::Window>();
		this->window->CreateWindowHandle(800, 600, "Sandbox");

		this->renderer = std::make_unique<Engine::GraphicsRenderer>(*window.get());

		
	};

	Application::~Application() 
	{
		this->renderer.reset();

		this->window->DestroyWindowHandle();
		this->window.reset();
	};

	void Application::TickInternal() 
	{
		if (this->window->IsHandleValid()) {
			this->window->PollEvents();
			this->renderer->Draw();
		}	
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}


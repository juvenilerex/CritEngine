#include "Application.h"
#include "Logging/Logger.h"


namespace Engine {

	Application::Application()
	{
		this->window = std::make_unique<Engine::Window>();
		this->window->CreateWindowHandle(800, 600, "Sandbox");

		this->input = std::make_unique<Engine::InputListener>(*window.get());
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

	bool Application::GetKeyDown(int key) {
		return this->input->GetKeyDown(key);
	}

	bool Application::GetKeyReleased(int key) {
		return this->input->GetKeyUp(key);
	}

	bool Application::GetKeyJustPressed(int key) {
		return this->input->GetKeyJustPressed(key);
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}


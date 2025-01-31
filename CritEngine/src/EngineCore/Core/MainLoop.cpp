
#include "MainLoop.h"

namespace Engine {

	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance) {
		Engine::LogInfo("MainLoop", "Starting");
		this->application = std::move(applicationInstance);
	};

	void MainLoop::Run() {

		window = std::make_unique<Engine::Window>(800, 600, "CritEngine");
		renderer = std::make_unique<Engine::GraphicsRenderer>(*window.get());

		this->is_running = true;
		
		while (is_running) { 
			Tick();
		}

	};

	void MainLoop::Stop() {
		this->is_running = false;
	};

	void MainLoop::Tick() {
		this->application->Tick();
		this->window->PollEvents();
		this->renderer->Draw();
	};

}

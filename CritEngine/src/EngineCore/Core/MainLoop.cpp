
#include "MainLoop.h"

namespace Engine {

	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance) {
		Engine::LogInfo("MainLoop", "Huh");
		this->application = std::move(applicationInstance);
	};

	std::shared_ptr<Engine::Window> window;

	void MainLoop::Run() {

		window = std::make_shared<Engine::Window>(800, 600, "CritEgine");

		this->is_running = true;

		std::ostringstream get_the_address;
		get_the_address << this->application;
		Engine::LogInfo("MainLoop", get_the_address.str());
		
		while (is_running) { 
			Tick();
		}

	};

	void MainLoop::Stop() {
		this->is_running = false;
	};

	void MainLoop::Tick() {
		//LogInfo("MainLoop", "Tick!");
		this->application->Tick();
	};

	std::shared_ptr<Engine::Window> GetEngineWindow()
	{
		return window;
	}

}

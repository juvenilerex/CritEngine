
#include "MainLoop.h"
#include "../Logging/Logger.h"

namespace Engine {

	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance) {
		Engine::LogInfo("MainLoop", "Huh");
		this->application = std::move(applicationInstance);
	};

	void MainLoop::Run() {
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
}


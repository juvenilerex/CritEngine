
#include "MainLoop.h"
#include "../Logging/Logger.h"
#include "../Window.h"

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

		Window window(800, 600, "CritEngine");

		while (is_running && !window.shouldClose()) {
			// Render stuff here (?)
			glClear(GL_COLOR_BUFFER_BIT);

			window.swapBuffers();
			window.pollEvents();

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

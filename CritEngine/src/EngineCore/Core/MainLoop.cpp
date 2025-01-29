
#include "MainLoop.h"
#include "../Logging/Logger.h"
#include "../Window.h"
#include "../Graphics.h"

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
		GraphicsRenderer graphicsRenderer(window);


//		window.windowCloseEvent.AddListener([this]() { MainLoop::Stop(); LogWarning("Event", "Thing."); });
		window.windowCloseEvent.AddListener([&window](){
			
			window.Test();
			
			});
		

		while (is_running && !window.ShouldClose()) { 
			// Render stuff here (?)
			graphicsRenderer.Draw();
			graphicsRenderer.PollEvents();

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

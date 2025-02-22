
#include "MainLoop.h"

namespace Engine {


	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance)
	{
		LogInfo("MainLoop", "Starting");
		this->application = std::move(applicationInstance);
	};

	void MainLoop::Run()
	{
		this->is_running = true;


		while (is_running)
		{
			Tick();
		}

	};

	void MainLoop::Stop()
	{
		this->is_running = false;
	};


	void MainLoop::Tick()
	{
		this->application->TickInternal();
		this->application->Tick();
	};

}

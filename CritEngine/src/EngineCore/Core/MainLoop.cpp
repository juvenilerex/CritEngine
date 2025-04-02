
#include "MainLoop.h"

namespace Engine {

	bool MainLoop::is_running = true;

	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance)
	{
		GlobalEngine::Initialize(std::move(applicationInstance));
	};

	void MainLoop::Run()
	{
		LogInfo("MainLoop", "Starting");

		while (is_running)
		{
			Tick();
		}

	};

	void MainLoop::Stop()
	{
		is_running = false;
	};

	void MainLoop::Tick()
	{
		GlobalEngine::Get().Tick();
	};
}

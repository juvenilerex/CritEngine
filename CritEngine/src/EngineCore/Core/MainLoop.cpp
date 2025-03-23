
#include "MainLoop.h"

namespace Engine {

	MainLoop::MainLoop(std::unique_ptr<Application> applicationInstance)
	{
		GlobalEngine::Initialize(std::move(applicationInstance));
	};

	void MainLoop::Run()
	{
		LogInfo("MainLoop", "Starting");
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
		GlobalEngine::Get().Tick();
	};
}

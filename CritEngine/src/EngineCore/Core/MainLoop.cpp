
#include "MainLoop.h"
#include "GlobalEngine.h"

namespace Engine {

	MainLoop::MainLoop()
	{
	};

	void MainLoop::Run()
	{
		LogInfo("MainLoop", "Starting");
    this->is_running = true;
		while (is_running)
		{
			this->Tick();
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

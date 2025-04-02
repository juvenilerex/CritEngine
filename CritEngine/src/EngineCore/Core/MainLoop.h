#pragma once

#include <memory>

#include "Base.h"
#include "GlobalEngine.h"

#include "../Logging/Logger.h"


namespace Engine
{
	class MainLoop
	{
	public:
		ENGINE_API MainLoop(std::unique_ptr<Application> applicationInstance);
		ENGINE_API void Run();
		ENGINE_API void Tick();
		ENGINE_API static void Stop();

	private:
		static bool is_running;
	};
}




#pragma once

#include <memory>

#include "../Core/Base.h"
#include "../Application.h"
#include "../Logging/Logger.h"


namespace Engine
{


	class MainLoop
	{
	public:
		ENGINE_API MainLoop(std::unique_ptr<Application> applicationInstance);
		ENGINE_API void Run();
		ENGINE_API void Tick();
		ENGINE_API void Stop();
	private:
		bool is_running = true;
		std::unique_ptr<Application> application = nullptr;
	};
}




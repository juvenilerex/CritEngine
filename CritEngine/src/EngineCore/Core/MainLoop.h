#pragma once

#include <memory>


#include "../Core.h"
#include "../Application.h"
#include "../Logging/Logger.h"
#include "../Graphics.h"
#include "../Window.h"


namespace Engine {

	std::shared_ptr<Engine::Window> GetEngineWindow();

	class ENGINE_API MainLoop {
	public:
		MainLoop(std::unique_ptr<Application> applicationInstance);
		void Run();
		void Tick();
		void Stop();
	private:
		bool is_running = true;

		std::unique_ptr<Application> application = nullptr;
	};
}




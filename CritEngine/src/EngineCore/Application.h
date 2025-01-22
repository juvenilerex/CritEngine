#pragma once

#include <memory>

#include "Core.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void virtual Tick();

	private:
		
	};

	std::unique_ptr<Application> CreateApplication();
}




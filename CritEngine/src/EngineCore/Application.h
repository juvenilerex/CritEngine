#pragma once

#include <memory>

#include "Core.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		static std::unique_ptr<Application> Create();
		Application();
		virtual ~Application();

		void Run();

	private:
		
	};
}




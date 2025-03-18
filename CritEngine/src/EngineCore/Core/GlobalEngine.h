#pragma once

#include "../Resource/ResourceManager.h"
#include "../Application.h"

namespace Engine {

	class GlobalEngine
	{
	public:
		GlobalEngine(std::unique_ptr<Application> injectedApp = std::make_unique<Application>()) 
			: resourceManager(ResourceManager()), application(std::move(injectedApp))
		{
		};

		ResourceManager& GetResourceManager();

		void Tick();

	private:
		ResourceManager resourceManager;
		std::unique_ptr<Application> application = nullptr;
	};

}
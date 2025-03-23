#pragma once

#include "../Resource/ResourceManager.h"
#include "../Application.h"

namespace Engine {

	class GlobalEngine
	{
	public:
		ENGINE_API static GlobalEngine& Get();
		static void Initialize(std::unique_ptr<Application> injectedApp = std::make_unique<Application>());

		ENGINE_API ResourceManager&  GetResourceManager();

		void Tick();

	private:
		GlobalEngine();

		static std::unique_ptr<GlobalEngine> globalInstance;

		ResourceManager resourceManager;
		std::unique_ptr<Application> application = nullptr;
	};

}
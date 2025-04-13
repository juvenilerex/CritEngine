#pragma once

#include "../Resource/ResourceManager.h"
#include "../Application.h"
#include "MainLoop.h"

namespace Engine {

	class GlobalEngine
	{
	public:
		ENGINE_API static GlobalEngine& Get();
		ENGINE_API static void Initialize(std::unique_ptr<Application> injectedApp = std::make_unique<Application>(), MainLoop mainLoop = MainLoop());
		ENGINE_API static void Shutdown();

		ENGINE_API ResourceManager&  GetResourceManager();

		void Tick();

	private:
		GlobalEngine();

		static std::unique_ptr<GlobalEngine> globalInstance;

		MainLoop mainLoop;
		ResourceManager resourceManager;
		std::unique_ptr<Application> application = nullptr;
	};

}
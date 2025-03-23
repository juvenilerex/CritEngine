#include "GlobalEngine.h"

#include "../Resource/Loaders/BitmapLoader.h"

namespace Engine {

	void GlobalEngine::Initialize(std::unique_ptr<Application> injectedApp)
	{
		std::unique_ptr<GlobalEngine> engine = std::make_unique<GlobalEngine>(GlobalEngine());

		engine->application = std::move(injectedApp);

		engine->resourceManager = ResourceManager();
		engine->resourceManager.RegisterLoader(std::make_unique<BitmapLoader>());

		GlobalEngine::globalInstance = std::move(engine);
		GlobalEngine::globalInstance->application->Initialize();
	};

	GlobalEngine& GlobalEngine::Get()
	{
		return *GlobalEngine::globalInstance.get();
	}

	ResourceManager& GlobalEngine::GetResourceManager()
	{
		return this->resourceManager;
	}

	void GlobalEngine::Tick()
	{
		this->application->TickInternal();
		this->application->Tick();
	}

	GlobalEngine::GlobalEngine()
	{
	}

	std::unique_ptr<GlobalEngine> GlobalEngine::globalInstance = nullptr;
}
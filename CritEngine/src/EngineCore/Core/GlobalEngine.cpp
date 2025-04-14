#include "GlobalEngine.h"

#include "../Resource/Loaders/BitmapLoader.h"

namespace Engine {

	void GlobalEngine::Initialize(std::unique_ptr<Application> injectedApp, MainLoop mainLoop)
	{
		std::unique_ptr<GlobalEngine> engine = std::make_unique<GlobalEngine>(GlobalEngine());

		engine->application = std::move(injectedApp);
		engine->mainLoop = mainLoop;

		engine->resourceManager = ResourceManager();
		engine->resourceManager.RegisterLoader(std::make_unique<BitmapLoader>());

		GlobalEngine::globalInstance = std::move(engine);
		GlobalEngine::globalInstance->application->Initialize();

		GlobalEngine::globalInstance->mainLoop.Run();
	}

	void GlobalEngine::Shutdown()
	{
		GlobalEngine::globalInstance->mainLoop.Stop();
	}

	GlobalEngine& GlobalEngine::Get()
	{
		return *GlobalEngine::globalInstance;
	}

	ResourceManager& GlobalEngine::GetResourceManager()
	{
		return this->resourceManager;
	}

	void GlobalEngine::Tick()
	{
		this->application->TickInternal();
		this->application->GetWindow().ImGuiStartFrame();
		this->application->Tick();
		this->application->GetWindow().ImGuiRender();
		//this->application->OnGUIUpdate();
		
	}

	GlobalEngine::GlobalEngine()
	{
	}

	std::unique_ptr<GlobalEngine> GlobalEngine::globalInstance = nullptr;
}
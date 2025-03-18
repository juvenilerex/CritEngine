#include "GlobalEngine.h"

namespace Engine {

	ResourceManager& GlobalEngine::GetResourceManager()
	{
		return this->resourceManager;
	}

	void GlobalEngine::Tick()
	{
		this->application->TickInternal();
		this->application->Tick();
	}
}
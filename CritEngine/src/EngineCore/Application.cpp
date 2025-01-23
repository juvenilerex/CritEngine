#include "Application.h"
#include "Logging/Logger.h"


namespace Engine {

	Application::Application()
	{

	};

	Application::~Application() 
	{

	};

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}


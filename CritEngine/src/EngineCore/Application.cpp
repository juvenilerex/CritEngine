#include "Application.h"

namespace Engine {

	std::unique_ptr<Application> Application::Create() {
		return std::make_unique<Application>();
	}

	Application::Application() 
	{

	}

	Application::~Application() 
	{

	};

	void Application::Run()
	{
		while (true);
	}

}


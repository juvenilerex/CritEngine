#pragma once

#include "Application.h"

int main(int argc, char** argv)
{
	std::unique_ptr<Engine::Application> Application = Engine::Application::Create();
	Application->Run();
}


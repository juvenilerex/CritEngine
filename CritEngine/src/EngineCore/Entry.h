#pragma once

#include "Application.h"

extern std::unique_ptr<Engine::Application> Engine::CreateApplication();

int main(int argc, char** argv)
{
	std::unique_ptr<Engine::Application> Application = Engine::CreateApplication();
	Application->Run();
}


#pragma once

#include <cstdio>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		printf("Hello World!");
	}

	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
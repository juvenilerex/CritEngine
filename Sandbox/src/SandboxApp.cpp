#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Entry.h>

class Sandbox : public Engine::Application
{

public:
	int test = 10;
	Sandbox()
	{
		printf("Hello");
		//main();
		std::ostringstream get_the_address;
		get_the_address << this;
		Engine::LogInfo("Sandbox", get_the_address.str());
	}

	void Tick() override {
		Engine::LogInfo("Sandbox", "Tock!");
	};

	int main() {
		return 0;
	}

	~Sandbox()
	{
		Engine::LogWarning("Sandbox", "Destroyed!");
	}
};

std::unique_ptr<Engine::Application> CreateApplication() {
	return std::make_unique<Sandbox>(Sandbox());
};
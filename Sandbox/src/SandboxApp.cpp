#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Input.h>

class Sandbox : public Engine::Application
{

public:


	void Tick() override {

		// We don't have access to GLFW's key enums, so we may need to import them (?)
		if (GetKeyDown(66)) { // If B is pressed
			Engine::LogWarning("Input", "B pressed!");
		}
		if (GetKeyReleased(66)) { // If B is released
			Engine::LogError("Input", "B released!");
		}

		Engine::LogInfo("Sandbox", "Tick!");
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
	std::unique_ptr<Sandbox>(new Sandbox());
	return std::make_unique<Sandbox>();
};
#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Graphics.h>

class Sandbox : public Engine::Application
{

public:

	std::shared_ptr<Engine::Window> window;
	std::shared_ptr<Engine::GraphicsRenderer> renderer = nullptr;

	Sandbox()
	{
		window = std::make_shared<Engine::Window>(800, 600, "CritEgine");
		renderer = std::make_shared<Engine::GraphicsRenderer>(*window);

		std::ostringstream get_the_address;
		get_the_address << this;
		Engine::LogInfo("Sandbox", get_the_address.str());

		window->windowCloseEvent.AddListener([]() {
			});

	}

	void Tick() override {
		renderer->Draw();
		renderer->PollEvents();
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
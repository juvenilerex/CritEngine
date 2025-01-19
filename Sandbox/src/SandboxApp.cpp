#pragma once

#include <cstdio>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Logging/Logger.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		Engine::Logger::Info("Sandbox", "Thing found.");
		Engine::Logger::Error("Sandbox", "Error found.");
		Engine::Logger::Warning("Sandbox", "Warning found.");
	}

	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
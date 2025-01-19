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
		LogInfo("Sandbox", "Thing found.");
		LogError("Sandbox", "Error found.");
		LogWarning("Sandbox", "Warning found.");
	}

	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
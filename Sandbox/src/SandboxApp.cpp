#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>

void SampleRawFunction() {
	Engine::LogWarning("Events", "Raw Function ran");
}

class Sandbox : public Engine::Application
{

public:
	Sandbox()
	{
		main();
	}

	void SampleMemberFunction() {
		Engine::LogWarning("Events", "Member Function ran");
	}

	int main() {

		Engine::EventEmitter eventEmitter = Engine::EventEmitter();

		eventEmitter.AddListener([]() {
			Engine::LogWarning("Events", "Lambda ran");
			}
		);
		
		std::function<void()> raw = &SampleRawFunction;
		eventEmitter.AddListener(raw);

		std::function<void()> member = std::bind(&Sandbox::SampleMemberFunction, this);
		eventEmitter.AddListener(member);

		eventEmitter.Emit();
		
		return 0;
	}



	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
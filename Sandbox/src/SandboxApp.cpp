#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <sstream>

#include <EngineCore/Layer.h>
#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Window/InputMouse.h>
#include <EngineCore/Window/Input.h>
#include <EngineCore/Entry.h>

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {
		
	}

	void OnUpdate() override {
		LogInfo("ExampleLayer", "Update");
	}

};

class Sandbox : public Engine::Application
{

public:

	Sandbox() {
		PushLayer(new LayerTest());

		Engine::EventEmitterBase emitterVoid = Engine::EventEmitterBase();
		Engine::EventEmitterBase<int> emitterInt = Engine::EventEmitterBase<int>();

		emitterVoid.AddListener([]()
		{
			std::stringstream oss;
			oss << "Hello Void";
			LogInfo("Event", oss.str());
			return;
		});

		emitterInt.AddListener([](int e)
		{
			std::stringstream oss;
			oss << "Hello " << e;
			LogInfo("Event", oss.str());
			return;
		});

		emitterInt.AddListener([](int e)
		{
			std::stringstream oss;
			oss << "Hello Other Event " << e;
			LogInfo("Event", oss.str());
			return;
		});

		emitterVoid.Emit();
		emitterInt.Emit(4000000);
	}

	void Tick() override
	{
		// We don't have access to GLFW's key enums, so we may need to import them (?)
		if (this->GetWindow().GetInput().GetKeyDown(66))
		{ // If B is pressed
			 LogWarning("Input", "B pressed!");
		}
		if (this->GetWindow().GetInput().GetKeyUp(66))
		{ // If B is released
			LogWarning("Input", "B released!");
		}
		if (this->GetWindow().GetInput().GetKeyJustPressed(67))
		{
			LogWarning("Input", "C just pressed!");
		}
		if (this->GetWindow().GetMouseButtonInput().GetMouseDown(1))
		{ // If mouse1 is pressed
			LogWarning("Input", "Mouse 1 pressed!");
		}
		if (this->GetWindow().GetMouseButtonInput().GetMouseJustPressed(0))
		{ // If mouse1 is pressed
			LogWarning("Input", "Mouse 0 just pressed!");
		}
		if (this->GetWindow().GetMouseButtonInput().GetMouseUp(1))
		{ // If mouse1 is pressed
			LogWarning("Input", "Mouse 1 just released!");
		}

		std::string mousePosX = std::to_string(this->GetWindow().GetMousePosition().X());
		std::string MousePosY = std::to_string(this->GetWindow().GetMousePosition().Y());
		std::stringstream ms;
		ms << mousePosX << ", " << MousePosY;

		LogInfo("MousePosition:", ms.str());
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}
};

std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
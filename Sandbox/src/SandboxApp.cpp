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
#include <EngineCore/Math/Vector.h>

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
		if (this->GetWindow().GetInput().GetKeyJustPressed(GLFW::KEY_SPACE))
		{ // If B is pressed
			Vector2 a(1.1f, 10.3f);
			LogWarning("Input", a.ToString());
			Vector2 b(1.63f, 6.3f);
			LogWarning("Input 2", b.ToString());
			Vector2 c = a + b;
			LogError("Input 3", c.ToString());
		}
		if (this->GetWindow().GetMouseInput().GetMouseJustPressed(GLFW::MOUSE_BUTTON_1)) {
			LogWarning("Mouse", "Mouse 1 pressed!");
		}

//		std::string mousePosX = std::to_string(this->GetWindow().GetMousePosition().X());
//		std::string MousePosY = std::to_string(this->GetWindow().GetMousePosition().Y());
//		std::stringstream ms;
//		ms << mousePosX << ", " << MousePosY;

		LogInfo("MousePosition:", this->GetWindow().GetMouseInput().GetPosition().ToString());
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
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

	std::unique_ptr<Engine::EventBus> bus;
	Engine::EventEmitterBase<int> intEmitter;
	Engine::EventEmitterBase<> voidEmitter;

	Sandbox() {
		PushLayer(new LayerTest());
		bus = std::make_unique<Engine::EventBus>();
		bus->AddEmitter("emit1", &voidEmitter);
		bus->AddEmitter("emit2", &intEmitter);

		bus->AddListener("emit1", { []() {
			LogInfo("voidEmitter", "Void emitter emitted"); 
			}});

		bus->AddListener<int>("emit2", { [](int i) {
			LogInfo("intEmitter", std::to_string(i));
			}});

		bus->Emit("emit2", 600);
		bus->Emit("emit2", 1000);
		bus->Emit("emit1");
		bus->Emit("emit2", 1);
		bus->RemoveListener<int>("emit2", { [](int i) {
			LogInfo("intEmitter", std::to_string(i));
			}});
		bus->Emit("emit2", 6);
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
		bus->Emit("emit1");
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
#pragma once

#include <cstdio>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event.h>

// Inherits from the Listener and takes on any behavior when notified
class EventListener : public Listener { 
	public:
		void onNotify(const std::string& event) override {
			LogInfo("Events", std::string(event + " notified"));			
		}
};

// A duplicate to test functionality
class SecondEventListener : public Listener {
public:
		void onNotify(const std::string& event) override {
			if (event != "Juump") {
				LogWarning("Events", std::string(event + " notified again!"));
			}
			else {
				LogWarning("Events", std::string(event + " notified again with two u's!"));
			}
		}
};

// A test object (i.e, a player) that notifies any attached listeners with an event 
class TestObject : public Emitter {
public:
	void Jump() {
		notify("Jump");
		notify("Juump");
	}
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		main();
	}

	int main() {
		EventListener eventListener; // A listener object
		SecondEventListener secondEventListener;
		TestObject testObject; // Test object, like a player

		testObject.addListener(&eventListener); // Adding listeners to test object
		testObject.addListener(&secondEventListener);

		testObject.Jump(); // Peforming just a function. We can pass event names manually as well


		LogInfo("Sandbox", "Thing found.");
		LogError("Sandbox", "Error found.");
		LogWarning("Sandbox", "Warning found.");

		return 0;
	}

	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

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

		testObject.addListener(&eventListener);
		testObject.addFunction("Thing", [this]() {this->Fun(); }); // Weird lambda stuff but it all breaks if it's not there
		testObject.addFunction("Thing2", [this]() {this->Fun2(); }); 
		testObject.addFunction("Thing3", [this]() {this->Fun3(); }); 


		testObject.triggerFunction("Thing"); // A small side effect here is if the event name is incorrect, it'll always
		                                  // default to the first event added
		testObject.triggerFunction("Thing2");
		testObject.triggerFunction("Thing3");


		testObject.Jump(); // Performing just a function. We can pass event names manually as well


		LogInfo("Sandbox", "Thing found.");
		LogError("Sandbox", "Error found.");
		LogWarning("Sandbox", "Warning found.");

		return 0;
	}

	void Fun(){
		LogWarning("Events", "Function ran");
	}
	void Fun2() {
		LogWarning("Events", "Function 2 ran");
	}
	void Fun3() {
		LogWarning("Events", "Function 3 ran");
	}


	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Logging/Logger.h>
#include "EngineCore/Event.h"


using namespace Engine;


class TestObject {
public:
	void TestEmit(){
		emit("Fun"); // We send out signal to the listeners to perform a function
		emit("Fun2");
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

		TestObject testObject;

		AddListener("Fun", Fun); /* Just a slight abstraction of the original function (can be found in Event.h)
									Listens for a signal that an object will emit, and runs a function
								 */
		AddListener("Fun2", Fun2);
		
		testObject.TestEmit(); 
		return 0;
	}

	void Fun(){
		LogWarning("Events", "Function ran");
	}
	void Fun2() {
		LogWarning("Events", "Function 2 ran");
	}

	~Sandbox()
	{

	}
};

std::unique_ptr<Engine::Application> Engine::CreateApplication() {
	return std::make_unique<Engine::Application>(Sandbox());
};
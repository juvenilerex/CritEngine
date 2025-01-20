#pragma once

#include <cstdio>
#include <functional>
#include <iostream>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event.h>

using namespace Engine;

// Any game object inheriting Events can emit signals
class TestObject {
public:
	void TestEmit(){
		emit("Fun"); // We send out signal to the listeners to perform a function
		emit("Fun3");
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

		addListener("Fun", [this]() {this->Fun();  }); // This lambda expression is the glue but is ugly
		addListener("Fun2", [this]() {this->Fun2(); });
		addListener("Fun3", [this]() {this->Fun3(); });

		testObject.TestEmit(); 
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
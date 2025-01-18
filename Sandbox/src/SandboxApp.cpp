#pragma once

#include <cstdio>

#include <EngineCore/Application.h>
#include <EngineCore/Entry.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		printf("Hello World!");
	}

	~Sandbox()
	{

	}
};
#pragma once

#include "Core/MainLoop.h"

extern std::unique_ptr<Engine::Application> CreateApplication();

int main(int argc, char** argv)
{
	Engine::MainLoop loop = Engine::MainLoop(std::move(CreateApplication()));
	loop.Run();
}
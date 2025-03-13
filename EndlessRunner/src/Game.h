#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <chrono>

#include <EngineCore/Layer.h>
#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Window/Input.h>
#include <EngineCore/Window/InputMouse.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Math/Vector2.h>
#include <EngineCore/Graphics/Renderer.h>
#include <EngineCore/Graphics/Scene.h>
#include <EngineCore/Graphics/Camera.h>
#include <EngineCore/Math/Vector3.h>

struct Transform2D {
	Engine::Vector2 position;
	Engine::Quaternion rotation;
	float scale;

	Transform2D() : position(Engine::Vector2()), rotation(Engine::Quaternion()), scale(1.0f) {}
};

class GameObject2D {

public:
	Transform2D transform;
};

class Player : public GameObject2D {

};

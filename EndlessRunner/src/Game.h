#pragma once

#include <cstdio>
#include <thread>
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
#include <thread>


struct Transform {
	Engine::Vector3 position;
	Engine::Quaternion rotation;
	float scale;

	Transform() : position(Engine::Vector3()), rotation(Engine::Quaternion()), scale(1.0f) {}
};

class GameObject {

public:
	Transform transform;

	GameObject() : transform() {}
    virtual ~GameObject() {}
};

class Player : public GameObject {

public:
    Engine::Vector3 velocity;
    bool isGrounded = false;
};


class ObjectSpawner {

public:

    void SpawnGameObjects() {

    }

private:

};

class Time {
public:

    static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f; // Must make it constexpr because of the in-class intialization

    static float deltaTime() {
        return FIXED_DELTA_TIME;
    }

    static void Update() {
        auto now = std::chrono::high_resolution_clock::now();
        static auto lastTime = now;

        auto elapsed = std::chrono::duration<float>(now - lastTime).count();

        if (elapsed < FIXED_DELTA_TIME) {
            auto sleepDuration = std::chrono::duration<float>(FIXED_DELTA_TIME - elapsed);
            std::this_thread::sleep_for(sleepDuration);
        }
        lastTime = now;
    }

private:
    Time(); // Private constructor to prevent instantiation, since this class is purely a utility
};

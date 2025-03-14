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

#include <random>


struct Transform {
	Engine::Vector3 position;
	Engine::Quaternion rotation;
	Engine::Vector3 scale;

	Transform() : position(Engine::Vector3()), rotation(Engine::Quaternion()), scale(Engine::Vector3()) {}
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

class AABB;

class Obstacle : public GameObject {

public:
    std::unique_ptr<AABB> box;
    Obstacle() : box(std::make_unique<AABB>(Engine::Vector2(0, 0), Engine::Vector2(0, 0))) {
       
    }
};

// Simple random number generator for general use
class RNG {

public:

    RNG() {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // Seed will be based on time
        rng.seed(static_cast<unsigned int>(seed));
    }

    inline float GetRandFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    inline int GetRandInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

private:
    // This looks so weird to me but it's apparently standard
    std::mt19937 rng;
};

class ObjectSpawner {

public:

    // This could definitely be expanded upon
    template<typename T>
    void Instantiate(std::shared_ptr<T> obj) {
        obj = std::make_shared<T>();
        objects.push_back(obj);
    }

    inline const std::vector<std::shared_ptr<GameObject>>* GetObjects() {
        return &this->objects;
    }

    const std::shared_ptr<GameObject>& GetLastObject() const {
        return this->objects.back();
    }

    void DeleteObject(const std::shared_ptr<GameObject>& obj) {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end()) {
            objects.erase(it);
        }
    }

private:
    std::vector<std::shared_ptr<GameObject>> objects;
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

class Shape2D {
public:
    virtual ~Shape2D() {}
    virtual bool isColliding(const Shape2D& other) const = 0;
};

// Super bare-bones AABB class for the purposes of this game
class AABB : public Shape2D {
public:
    Engine::Vector2 position;
    Engine::Vector2 size;
    
    AABB(const Engine::Vector2& position, const Engine::Vector2& size) : position(position), size(size) {}

    bool isColliding(const Shape2D& other) const override {
        const AABB* otherAABB = dynamic_cast<const AABB*>(&other);
        ASSERT(otherAABB, "Collision detection with unsupported shape!"); // Right now this is only compatible between other AABB's

        // AABB collision detection. We are dividing the size by 2 since the bounding box's position represents
        // the center of the box and collisions seem inaccurate unless it's done this way
        return (position.x - size.x / 2 < otherAABB->position.x + otherAABB->size.x / 2 &&
            position.x + size.x / 2 > otherAABB->position.x - otherAABB->size.x / 2 &&
            position.y - size.y / 2 < otherAABB->position.y + otherAABB->size.y / 2 &&
            position.y + size.y / 2 > otherAABB->position.y - otherAABB->size.y / 2);
    }
};  
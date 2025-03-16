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

#include <random>

template<typename T>
class Transform {

public:
	T position, scale;
	Engine::Quaternion rotation;

    Transform() : position(T()), rotation(Engine::Quaternion()), scale(T()) {}

};

class GameObject2D {

public:
    virtual ~GameObject2D() = default;
    Transform<Engine::Vector2> transform;

};

class BoundingBox;

class Player2D : public GameObject2D {

public:
	bool isJumping = false;
	bool isGrounded = false;

    Player2D(Engine::Vector2 position, Engine::Vector2 scale) : boundingBox(std::make_unique<BoundingBox>(position.x, position.y, scale.x, scale.y)) {
        transform.position = position;
        transform.scale = scale;
    }

    Engine::Vector2 velocity;
    std::unique_ptr<BoundingBox> boundingBox;
};


class Obstacle : public GameObject2D {

public:

    Obstacle(Engine::Vector2 position, Engine::Vector2 scale) : boundingBox(std::make_unique<BoundingBox>(position.x, position.y, scale.x, scale.y)) {

    }

    std::unique_ptr<BoundingBox> boundingBox;
};

class Shape2D {
public:
    virtual ~Shape2D() {}
    virtual bool isColliding(const Shape2D& other) const = 0;
};

class BoundingBox {

public:
    BoundingBox(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {
    }

    inline void SetPosition(const Engine::Vector2 pos) {
        x = pos.x;
        y = pos.y;
    }

    inline void SetDimensions(const Engine::Vector2 dim) {
        width = dim.x;
        height = dim.y;
    }

    inline Engine::Vector2 GetPosition() {
        return Engine::Vector2(x, y);
    }

    inline Engine::Vector2 GetDimensions() {
        return Engine::Vector2(width, height);
    }

    bool isCollidingWith(const BoundingBox& other) const {
        return(x - width / 2 < other.x + other.width / 2 && 
            x + width / 2 > other.x - other.width / 2 &&  
            y - height / 2 < other.y + other.height / 2 && 
            y + height / 2 > other.y - other.height / 2); 
    }

private:
    float x, y, width, height;
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
        return (position.x < otherAABB->position.x + otherAABB->size.x &&
            position.x + size.x > otherAABB->position.x &&
            position.y < otherAABB->position.y + otherAABB->size.y &&
            position.y + size.y > otherAABB->position.y);
    }
};

class RandomNumberGenerator {

public:

    RandomNumberGenerator() {
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

    inline const std::vector<std::shared_ptr<GameObject2D>>* GetObjects() {
        return &this->objects;
    }

    const std::shared_ptr<GameObject2D>& GetLastObject() const {
        return this->objects.back();
    }

    void DeleteObject(const std::shared_ptr<GameObject2D>& obj) {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end()) {
            objects.erase(it);
        }
    }

private:
    std::vector<std::shared_ptr<GameObject2D>> objects;
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

class PrimitiveDraw {

public:
    static void Draw(const std::shared_ptr<Engine::Shader>& shader,
        const Engine::Matrix4f& mat4, const std::shared_ptr<Engine::VertexArray>& va) {
        
        shader->Bind();
        shader->UploadUniformMat4("uModelProjection", mat4);
        Engine::Renderer::Submit(shader, va);
        shader->Unbind();
    }

private:
    PrimitiveDraw();

};

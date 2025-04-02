#pragma once
#include "../Component.h"
#include "../../Math/Common.h"

struct PhysicsComponent2D : ECS::Component
{
	Engine::Vector2 velocity;
	Engine::Vector2 acceleration;
	Engine::Vector2 centerOfMass;
	float angularVelocity;
	float angularAcceleration;
	float mass;
	float friction;
	float elasticity;
	float drag;
	uint32_t collisionMask;
};

struct PhysicsComponent3D : ECS::Component
{
	Engine::Vector3 velocity;
	Engine::Vector3 acceleration;
	Engine::Vector3 angularVelocity;
	Engine::Vector3 angularAcceleration;
	Engine::Vector3 centerOfMass;
	float mass;
	float friction;
	float elasticity;
	float drag;
	uint32_t collisionMask;
};
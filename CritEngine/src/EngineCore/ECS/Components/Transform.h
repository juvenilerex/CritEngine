#pragma once
#include "../Component.h"
#include "../../Math/Common.h"

struct TransformComponent2D : ECS::Component
{
	float rotation;
	Engine::Vector2 position;
	Engine::Vector2 scale;
};

struct TransformComponent3D : ECS::Component
{
	Engine::Quaternion rotation;
	Engine::Vector3 position;
	Engine::Vector3 scale;
};
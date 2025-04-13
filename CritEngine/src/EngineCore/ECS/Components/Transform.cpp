#include "Transform.h"

Engine::Matrix4f TransformComponent2D::GetMatrix()
{
	float a = this->rotation;
	Engine::Vector2 p = this->position;

	Engine::Matrix4f transform = Engine::Matrix4f({
		cos(a), -sin(a), 0, 0,
		sin(a), cos(a), 0, 0,
		0, 0, 1, 0,
		p.x, p.y, 0, 1
	});

	return transform;
}

Engine::Matrix4f TransformComponent3D::GetMatrix()
{
	Engine::Quaternion q = this->rotation;
	Engine::Vector3 p = this->position;

	Engine::Matrix4f rotation = Engine::Matrix4f({
		1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
		2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
		2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
		0, 0, 0, 1
	});

	Engine::Matrix4f position = Engine::Matrix4f({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		p.x, p.y, p.z, 1
	});

	return rotation * position;
}

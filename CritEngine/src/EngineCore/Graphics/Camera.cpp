#include "Camera.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(
		float verticalFOV,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Vector3 position,
		Quaternion rotation
	)
		: verticalFOV(verticalFOV), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), position(position), rotation(rotation)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

	Matrix4f PerspectiveCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetViewMatrix() { return this->viewMatrix; }

	void PerspectiveCamera::CalculatePerspectiveMatrix()
	{
		float far = this->farPlane;
		float near = this->nearPlane;
		float fovRad = this->verticalFOV / 180.f * 3.14159265f;
		float tanFovY = tanf(fovRad / 2.f);

		this->perspectiveMatrix = Matrix4f({
			1 / (this->aspectRatio * tanFovY), 0, 0, 0,
			0, 1 / tanFovY, 0, 0,
			0, 0, (far + near) / (near + far), 1,
			0, 0, (2 * far * near) / (near - far), 0
		});

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}

	void PerspectiveCamera::CalculateViewMatrix()
	{
		Quaternion q = this->rotation;
		Vector3 p = this->position;

		// In the future the view matrix should be created in place to save on some cycles for every recalculation.

		Matrix4f rotation = Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			0, 0, 0, 1
		});

		Matrix4f position = Matrix4f({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-p.x, -p.y, -p.z, 1
		});

		this->viewMatrix = rotation * position;

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}
}
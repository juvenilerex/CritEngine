#include "Camera.h"

namespace Engine {

	////////////////////////////////////////////////////////
	//  Perspective Camera  ////////////////////////////////
	////////////////////////////////////////////////////////

	PerspectiveCamera::PerspectiveCamera(
		float verticalFOV,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Vector3 position,
		Quaternion rotation
	)
		: rotation(rotation), position(position), verticalFOV(verticalFOV), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

	Matrix4f PerspectiveCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetViewMatrix() { return this->viewMatrix; }

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

	void PerspectiveCamera::SetVerticalFOV(float verticalFOV)
	{
		this->verticalFOV = verticalFOV;
		this->CalculatePerspectiveMatrix();
	}
	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		this->aspectRatio = aspectRatio;
		this->CalculatePerspectiveMatrix();
	}
	void PerspectiveCamera::SetNearPlane(float nearPlane)
	{
		this->nearPlane = nearPlane;
		this->CalculatePerspectiveMatrix();
	}
	void PerspectiveCamera::SetFarPlane(float farPlane)
	{
		this->farPlane = farPlane;
		this->CalculatePerspectiveMatrix();
	}
	void PerspectiveCamera::SetPosition(Vector3 position)
	{
		this->position = position;
		this->CalculateViewMatrix();
	}
	void PerspectiveCamera::SetRotation(Quaternion rotation)
	{
		this->rotation = rotation;
		this->CalculateViewMatrix();
	}

	Vector3 PerspectiveCamera::GetPosition()
	{
		return this->position;
	}
	Quaternion PerspectiveCamera::GetRotation()
	{
		return this->rotation;
	}

	////////////////////////////////////////////////////////
	//  Orthographic Camera  ///////////////////////////////
	////////////////////////////////////////////////////////

	OrthographicCamera::OrthographicCamera(
		float height,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Vector3 position,
		Quaternion rotation
	)
		: rotation(rotation), position(position), height(height), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

	Matrix4f OrthographicCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Matrix4f OrthographicCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Matrix4f OrthographicCamera::GetViewMatrix() { return this->viewMatrix; }

	void OrthographicCamera::CalculateViewMatrix()
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

	void OrthographicCamera::CalculatePerspectiveMatrix()
	{

		float far = this->farPlane;
		float near = this->nearPlane;
		float top = this->height / 2;
		float bottom = -this->height / 2;
		float left = this->height * this->aspectRatio / 2;
		float right = -this->height * this->aspectRatio / 2;

		this->perspectiveMatrix = Engine::Matrix4f({
			-2 / (right - left), 0, 0, 0,
			0, 2 / (top - bottom), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1,
		});

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;

	}

	void OrthographicCamera::SetHeight(float height)
	{
		this->height = height;
		this->CalculatePerspectiveMatrix();
	}
	void OrthographicCamera::SetAspectRatio(float aspectRatio)
	{
		this->aspectRatio = aspectRatio;
		this->CalculatePerspectiveMatrix();
	}
	void OrthographicCamera::SetNearPlane(float nearPlane)
	{
		this->nearPlane = nearPlane;
		this->CalculatePerspectiveMatrix();
	}
	void OrthographicCamera::SetFarPlane(float farPlane)
	{
		this->farPlane = farPlane;
		this->CalculatePerspectiveMatrix();
	}
	void OrthographicCamera::SetPosition(Vector3 position)
	{
		this->position = position;
		this->CalculateViewMatrix();
	}
	void OrthographicCamera::SetRotation(Quaternion rotation)
	{
		this->rotation = rotation;
		this->CalculateViewMatrix();
	}

	Vector3 OrthographicCamera::GetPosition()
	{
		return this->position;
	}
	Quaternion OrthographicCamera::GetRotation()
	{
		return this->rotation;
	}
}
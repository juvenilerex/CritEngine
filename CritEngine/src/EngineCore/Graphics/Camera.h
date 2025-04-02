#pragma once

#include "../Math/Common.h"
#include "Scene.h"


namespace Engine {

	class ENGINE_API PerspectiveCamera
	{
	public:

		PerspectiveCamera(float verticalFOV, float aspectRatio, float nearPlane, float farPlane, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());

		Matrix4f GetViewPerspectiveMatrix();
		Matrix4f GetPerspectiveMatrix();
		Matrix4f GetViewMatrix();

		void CalculateViewMatrix();
		void CalculatePerspectiveMatrix();

	private:
		Matrix4f viewMatrix = Matrix4f::Identity();
		Matrix4f perspectiveMatrix = Matrix4f::Identity();
		Matrix4f viewPerspectiveMatrix = Matrix4f::Identity();

		Quaternion rotation;
		Vector3 position;
		float verticalFOV;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};		  

	class ENGINE_API OrthographicCamera
	{
	public:

		OrthographicCamera(float verticalFOV, float aspectRatio, float nearPlane, float farPlane, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());

		Matrix4f GetViewPerspectiveMatrix();
		Matrix4f GetPerspectiveMatrix();
		Matrix4f GetViewMatrix();

		void CalculateViewMatrix();
		void CalculatePerspectiveMatrix();

	private:
		Matrix4f viewMatrix = Matrix4f::Identity();
		Matrix4f perspectiveMatrix = Matrix4f::Identity();
		Matrix4f viewPerspectiveMatrix = Matrix4f::Identity();

		Quaternion rotation;
		Vector3 position;
		float height;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};

}
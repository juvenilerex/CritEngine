#pragma once

#include <memory>
#include "../Math/Common.h"

namespace Engine {
	class Scene
	{
	public:
		ENGINE_API static void SetActiveScene(std::shared_ptr<Scene> scene);
		ENGINE_API static std::shared_ptr<Scene> GetActiveScene();
	private:
		static std::shared_ptr<Scene> activeScene;

	public:
		Matrix4f viewProjectionMatrix;
		Matrix4f perspectiveProjectionMatrix;
		Matrix4f viewPerspectiveProjectionMatrix;
	};
}
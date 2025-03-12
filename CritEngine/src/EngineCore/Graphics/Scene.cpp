#include "Scene.h"

namespace Engine {
	void Scene::SetActiveScene(std::shared_ptr<Scene> scene)
	{
		Scene::activeScene = scene;
	}

	std::shared_ptr<Scene> Scene::GetActiveScene()
	{
		return Scene::activeScene;
	}

	std::shared_ptr<Scene> Scene::activeScene = nullptr;
}
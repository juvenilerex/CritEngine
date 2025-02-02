#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

	// Defining static key states
	std::unordered_map<int, bool> InputListener::KeyStates;
	std::unordered_map<int, bool> InputListener::KeyReleased;

	Engine::InputListener::InputListener(Engine::Window& window_ref) : windowHandle(window_ref.GetHandle())
	{
		glfwSetKeyCallback(windowHandle, KeyCallback);
	}

	void InputListener::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		if (action == GLFW_PRESS) {
			KeyStates[key] = true;
			KeyReleased[key] = false;
		}
		else if (action == GLFW_RELEASE) {
			KeyStates[key] = false;
			KeyReleased[key] = true;
		}

	}

	// If the key is being held down
	bool InputListener::GetKeyDown(int key) const {

		auto i = KeyStates.find(key);
		if (i != KeyStates.end()) {
			return i->second;
		}
		else {
			return false;
		}

	}

	// If the key was just released
	bool InputListener::GetKeyUp(int key) const {

		auto i = KeyReleased.find(key);
		if (i != KeyReleased.end() && i->second) {
			KeyReleased[key] = false;
			return true;
		}
		return false;

	}

}

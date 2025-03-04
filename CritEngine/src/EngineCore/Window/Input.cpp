#pragma once
#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

	// Defining static key states
	std::unordered_map<int, bool> InputListener::KeyStates;
	std::unordered_map<int, bool> InputListener::KeyReleased;

	InputListener::InputListener(GLFWwindow* windowHandle) : windowHandle(windowHandle)
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

		auto i = this->KeyStates.find(key);
		if (i != this->KeyStates.end()) {
			return i->second;
		}
		else {
			return false;
		}

	}

	// If the key was just released
	bool InputListener::GetKeyUp(int key) const {

		auto i = this->KeyReleased.find(key);
		if (i != this->KeyReleased.end() && i->second) {
			this->KeyReleased[key] = false;
			return true;
		}
		return false;

	}

	// If the key was just pressed down, only true for one frame
	bool InputListener::GetKeyJustPressed(int key) const {

		auto i = this->KeyStates.find(key);
		if (i != this->KeyStates.end() && i->second) {
			this->KeyStates[key] = false;
			return true;
		}
		return false;

	}

}

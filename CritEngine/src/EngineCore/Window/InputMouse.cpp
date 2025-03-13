#pragma once
#include "InputMouse.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

	// Defining static key states
	std::unordered_map<int, bool> MouseInputListener::ButtonStates, MouseInputListener::ButtonReleased;

	MouseInputListener::MouseInputListener(GLFWwindow* windowHandle) : windowHandle(windowHandle)
	{
		glfwSetMouseButtonCallback(windowHandle, MouseButtonCallback);
		glfwSetCursorPosCallback(windowHandle, CursorPosCallback);
	}

	void MouseInputListener::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
	{
		if (action == GLFW_PRESS) {
			ButtonStates[button] = true;
			ButtonReleased[button] = false;
		}
		else if (action == GLFW_RELEASE) {
			ButtonStates[button] = false;
			ButtonReleased[button] = true;
		}
	}

	// If the mouse button is being held down
	bool MouseInputListener::GetMouseDown(int button) const 
	{

		auto i = this->ButtonStates.find(button);
		if (i != this->ButtonStates.end()) {
			return i->second;
		}
		else {
			return false;
		}

	}

	// If the key was just released
	bool MouseInputListener::GetMouseUp(int button) const 
	{

		auto i = this->ButtonReleased.find(button);
		if (i != this->ButtonReleased.end() && i->second) {
			this->ButtonReleased[button] = false;
			return true;
		}
		return false;

	}

	// If the key was just pressed down, only true for one frame
	bool MouseInputListener::GetMouseJustPressed(int button) const 
	{

		auto i = this->ButtonStates.find(button);
		if (i != this->ButtonStates.end() && i->second) {
			this->ButtonStates[button] = false;
			return true;
		}
		return false;

	}

	// Defining static mouse position coordinates
	double MouseInputListener::posX, MouseInputListener::posY;

	Vector2 MouseInputListener::GetPosition() const {
		return Vector2((float)posX, (float)posY);
	}
	
	void MouseInputListener::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		posX = xpos; posY = ypos;
	}


}

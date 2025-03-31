#pragma once
#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Window/Window.h"

namespace Engine {

    std::unordered_map<int, bool> InputListener::KeyStates; 
    std::unordered_map<int, bool> InputListener::KeyReleased;
    std::unordered_map<int, bool> InputListener::KeyJustPressed;
    std::unordered_map<int, bool> InputListener::ButtonStates;
    std::unordered_map<int, bool> InputListener::ButtonReleased; 
    std::unordered_map<int, bool> InputListener::ButtonJustPressed;
    Vector2 InputListener::CurPos;
    Vector2 InputListener::PrevCurPos;
    bool InputListener::CurPosChanged = false;

    InputListener::InputListener(GLFWwindow* windowHandle) : windowHandle(windowHandle) {
        glfwSetKeyCallback(windowHandle, KeyCallback);
        glfwSetMouseButtonCallback(windowHandle, MouseButtonCallback);
        glfwSetCursorPosCallback(windowHandle, CursorPosCallback);
    }

    void InputListener::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (action == GLFW_PRESS) {
            KeyStates[key] = true;
            KeyReleased[key] = false;
            KeyJustPressed[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            KeyStates[key] = false;
            KeyReleased[key] = true;
        }

    }

    void InputListener::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {

        if (action == GLFW_PRESS) {
            ButtonStates[button] = true;
            ButtonReleased[button] = false;
            ButtonJustPressed[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            ButtonStates[button] = false;
            ButtonReleased[button] = true;
        }

    }

    void InputListener::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (CurPos.x != xpos || CurPos.y != ypos) {
            PrevCurPos = CurPos;
            CurPos.x = xpos;
            CurPos.y = ypos;
            CurPosChanged = true;
        }
        else {
            CurPosChanged = false;
        }
    }

    void InputListener::PollKeyEvents() const {

        for (const auto& [key, pressed] : KeyStates) {
            if (pressed) {
                KeyPressedEvent pressEvent(key);
                this->eventCallback(pressEvent);
            }
        }

        for (const auto& [key, released] : KeyReleased) {
            if (released) {
                KeyReleased[key] = false;
                KeyReleasedEvent releaseEvent(key);
                this->eventCallback(releaseEvent);
            }
        }

        for (const auto& [key, justPressed] : KeyJustPressed) {
            if (justPressed) {
                KeyJustPressed[key] = false;
                KeyJustPressedEvent justPressEvent(key);
                this->eventCallback(justPressEvent);
            }
        }

    }

    void InputListener::PollMouseEvents() const {

        for (const auto& [button, pressed] : ButtonStates) {
            if (pressed) {
                MousePressedEvent pressEvent(button);
                this->eventCallback(pressEvent);
            }
        }

        for (const auto& [button, released] : ButtonReleased) {
            if (released) {
                ButtonReleased[button] = false;
                MouseReleasedEvent releaseEvent(button);
                this->eventCallback(releaseEvent);
            }
        }

        for (const auto& [button, justPressed] : ButtonJustPressed) {
            if (justPressed) {
                ButtonJustPressed[button] = false;
                MouseJustPressedEvent justPressEvent(button);
                this->eventCallback(justPressEvent);
            }
        }

        if (CurPosChanged) {
            MouseMovedEvent moveEvent(CurPos.x, CurPos.y);
            this->eventCallback(moveEvent);
            CurPosChanged = false;
        }

    }

}

#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

    Window::Window() {

        if (!glfwInit()) {
            LogError("GLFW", "Failed to initialize GLFW!");
            exit(EXIT_FAILURE);
        }
    }

    Window::~Window() {

        glfwDestroyWindow(windowHandle);
        glfwTerminate();
    }

    bool Window::CreateWindowHandle(int width, int height, std::string title) {

        // We can use whatever version we want, I just set it to the latest
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!this->windowHandle) {
            LogError("GLFW", "Failed to create GLFW window!");
            return false;
        }

        glfwSetWindowUserPointer(this->windowHandle, this);
        glfwSetWindowCloseCallback(this->windowHandle, [](GLFWwindow* window) {
            Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(window));
            });

        return true;
    }

    void Window::DestroyWindowHandle() {
        glfwDestroyWindow(this->windowHandle);
        this->windowHandle = nullptr;
    }

    bool Window::IsHandleValid() {
        return this->windowHandle != nullptr;
    }

    GLFWwindow* Window::GetHandle() {
        return this->windowHandle;
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(this->windowHandle);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

};
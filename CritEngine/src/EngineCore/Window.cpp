#include "Window.h"
#include <iostream>

namespace Engine {

    Window::Window(int width, int height, std::string title) {

        initializeGLFW();
        createWindow(width, height, title);
        initializeGLAD();

    }

    Window::~Window() {

        glfwDestroyWindow(window);
        glfwTerminate();

    }

    void Window::initializeGLFW() {

        if (!glfwInit()) {
            LogError("GLFW", "Failed to initialize GLFW!");
            exit(EXIT_FAILURE);
        }

        // We can use whatever version we want, I just set it to the latest
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

    void Window::createWindow(int width, int height, std::string title) {

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!window) {
            LogError("GLFW", "Failed to create GLFW window!");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Wait on 1 monitor refresh before swapping buffers / VSync

    }

    void Window::initializeGLAD() {

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LogError("GLAD", "Failed to initialize GLAD!");
            exit(EXIT_FAILURE);
        }

    }

    bool Window::shouldClose()  {
        return glfwWindowShouldClose(window);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(window);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }
};
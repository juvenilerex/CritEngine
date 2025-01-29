#pragma once

#include "Logging/Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Engine {

    class ENGINE_API Window {

    public:

        Window(int width, int height, std::string title);
        ~Window();

        bool shouldClose();
        void swapBuffers();
        void pollEvents();

    private:

        GLFWwindow* window;

        void initializeGLFW();
        void initializeGLAD();
        void createWindow(int width, int height, std::string title);

    };

};
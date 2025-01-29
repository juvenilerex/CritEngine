#pragma once

#include "Event/Event.h"
#include "Logging/Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Engine {

    class ENGINE_API Window {

    public:

        EventEmitterBase windowCloseEvent;

        Window(int width, int height, std::string title);
        ~Window();

        bool shouldClose();
        void setShouldClose(int value);
        void swapBuffers();
        void Test();
        void pollEvents();

    private:

        GLFWwindow* window;

        void initializeGLFW();
        void initializeGLAD();
        void createWindow(int width, int height, std::string title);

    };

};
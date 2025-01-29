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

        bool ShouldClose();
        void SetShouldClose(int value);
        void SwapBuffers();
        void Test();
        void PollEvents();

    private:
        GLFWwindow* window;

        void InitializeGLFW();
        void InitializeGLAD();
        void createWindow(int width, int height, std::string title);

    };

};
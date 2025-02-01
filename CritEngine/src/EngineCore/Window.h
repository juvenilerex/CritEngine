#pragma once

#include "Event/Event.h"
#include "Logging/Logger.h"
#include <string>

struct GLFWwindow;

namespace Engine {

    class ENGINE_API Window {

    public:

        Window();
        ~Window();
        
        
        bool CreateWindowHandle(int width, int height, std::string title);
        void DestroyWindowHandle();

        bool IsHandleValid();
        GLFWwindow* GetHandle();

        void PollEvents();
        void SwapBuffers();

    private:
        GLFWwindow* windowHandle = nullptr;
        
        

    };

};
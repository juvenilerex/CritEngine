#pragma once
#include "Window.h"
#include "../Graphics/OpenGL/OpenGLContext.h"
#include "../Graphics/RenderContext.h"
#include "../Event/WindowEvent.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

	Window::Window(int width, int height, std::string title) 
		: width(width), height(height)
	{
		bool success = glfwInit();
		ASSERT(success, "Failed to initialize GLFW!");

		// We can use whatever version we want, I just set it to the latest
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		ASSERT(this->windowHandle, "Failed to create GLFW window!");

		glfwSetWindowUserPointer(this->windowHandle, this);

		this->input = std::make_unique<InputListener>(this->GetHandle());
		this->renderContext = std::make_unique<OpenGLContext>(this->GetHandle());
		this->renderContext->Init();


		glfwSetWindowCloseCallback(this->windowHandle, [](GLFWwindow* window)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			
			WindowCloseEvent _event;
			win->eventCallback(_event);
		});


		glfwSetWindowSizeCallback(this->windowHandle, [](GLFWwindow* window, int width, int height) 
		{
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));		

			win->SetWidth(width);
			win->SetHeight(height);

			WindowResizeEvent _event(width, height);
			win->eventCallback(_event);
		});

	}

	Window::~Window()
	{
		glfwDestroyWindow(this->windowHandle);
		glfwTerminate();
	}

	GLFWwindow* Window::GetHandle()
	{
		return this->windowHandle;
	}

	void Window::SwapBuffers()
	{
		this->renderContext->SwapBuffers();
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	InputListener& Window::GetInput()
	{
		ASSERT(this->input);
		return *this->input.get();
	}

};
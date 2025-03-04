#pragma once
#include "Window.h"
#include "../Graphics/OpenGL/OpenGLContext.h"
#include "../Graphics/RenderContext.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

	Window::Window(int width, int height, std::string title)
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
		glfwSetWindowCloseCallback(this->windowHandle, Window::WindowCloseCallback);
		
		this->eventBus = std::make_unique<EventBus>();
		this->mouseInput = std::make_unique<MouseInputListener>(this->GetHandle());
		this->input = std::make_unique<InputListener>(this->GetHandle());
		this->renderContext = std::make_unique<OpenGLContext>(this->GetHandle());
		this->renderContext->Init();

		eventBus->AddEmitter("WindowCloseEvent", &WindowCloseEvent);

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

	EventBus& Window::GetWindowEventBus()
	{
		return *this->eventBus;
	}

	void Window::WindowCloseCallback(GLFWwindow* window)
	{
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        win->eventBus->Emit("WindowCloseEvent");
	}

	MouseInputListener& Window::GetMouseInput()
	{
		ASSERT(this->mouseInput);
		return *this->mouseInput.get();
	}

};
#include "Graphics.h"
#include "Core/MainLoop.h" // Need this to reference the window context we created
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {


	GraphicsRenderer::GraphicsRenderer(Engine::Window& window_ref) 
		: window(window_ref)
	{
	}

	GraphicsRenderer::~GraphicsRenderer()
	{

	}

	// To be replaced with more relevant stuff here, but keeping this below for testing
	//
	void GraphicsRenderer::Draw()
	{
		glClearColor(0.6f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window.SwapBuffers();
	}

}
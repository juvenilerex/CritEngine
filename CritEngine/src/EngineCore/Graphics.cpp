#include "Graphics.h"

namespace Engine {

	GraphicsRenderer::GraphicsRenderer(Window& _window)
	{
		window = &_window; // We'll pass our main window into the constructor above
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
		window->SwapBuffers();
	}

	void GraphicsRenderer::PollEvents()
	{
		window->PollEvents();
	}

}
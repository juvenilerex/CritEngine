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
		glClear(GL_COLOR_BUFFER_BIT);
		window->swapBuffers();
	}

	void GraphicsRenderer::PollEvents()
	{
		window->pollEvents();
	}

}
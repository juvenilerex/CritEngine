#pragma once

#include "Window.h"

namespace Engine {

	class ENGINE_API GraphicsRenderer {

	public:
		GraphicsRenderer(Window& _window);
		~GraphicsRenderer();

		void Draw();
		void PollEvents();

	private:
		Window* window;
	};
}

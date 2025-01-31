#pragma once

#include "Window.h"

namespace Engine {

	class ENGINE_API GraphicsRenderer {

	public:
		GraphicsRenderer(Engine::Window& window);
		~GraphicsRenderer();

		void Draw();

	private:
		Engine::Window& window;
	};
}

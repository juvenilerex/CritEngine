#pragma once

#include "Window.h"

namespace Engine {

	class ENGINE_API GraphicsRenderer {

	public:
		GraphicsRenderer();
		~GraphicsRenderer();

		void Draw();
		void PollEvents();

	private:
		std::shared_ptr<Engine::Window> window;
	};
}

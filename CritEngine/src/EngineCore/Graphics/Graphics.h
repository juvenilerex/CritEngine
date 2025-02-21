#pragma once

#include "Shader.h"

template class ENGINE_API std::_Compressed_pair<std::default_delete<Engine::Shader>, Engine::Shader*, true>;
template class ENGINE_API std::unique_ptr<Engine::Shader>;

struct GLFWwindow;

namespace Engine {

	class ENGINE_API GraphicsRenderer
	{
	public:
		GraphicsRenderer(GLFWwindow* window);
		~GraphicsRenderer();

		void Draw();

	private:

		unsigned int vertexArray;
		std::unique_ptr<Shader> shader;
		GLFWwindow* windowHandle;
	};
}

#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

template class ENGINE_API std::_Compressed_pair<std::default_delete<Engine::Shader>, Engine::Shader*, true>;
template class ENGINE_API std::unique_ptr<Engine::Shader>;

struct GLFWwindow;

namespace Engine {

	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class GraphicsRenderer
	{
	public:
		ENGINE_API GraphicsRenderer(GLFWwindow* window);
		ENGINE_API ~GraphicsRenderer();
		ENGINE_API void Draw();

		inline static RendererAPI GetAPI() { return GraphicsRenderer::renderAPI; }
	private:
		static RendererAPI renderAPI;

		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		std::shared_ptr<VertexArray> squareVA;
		
		std::unique_ptr<Shader> shader;
		GLFWwindow* windowHandle;
	};
}

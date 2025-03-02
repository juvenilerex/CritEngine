#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace Engine {

	class Renderer
	{
	public:

		ENGINE_API static void BeginScene();
		ENGINE_API static void EndScene();

		ENGINE_API static void Submit(const std::shared_ptr<VertexArray>& vertexArray);


		ENGINE_API inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

	};
}

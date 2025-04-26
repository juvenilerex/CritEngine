#pragma once

#include "Scene.h"
#include "Camera.h"
#include "RenderCommand.h"

#include "PIL/Shader.h"
#include "PIL/Buffer.h"
#include "PIL/VertexArray.h"

namespace Engine {

	class Renderer
	{
	public:

		ENGINE_API static void BeginScene(std::shared_ptr<PerspectiveCamera> camera);
		ENGINE_API static void EndScene();

		ENGINE_API static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		ENGINE_API inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

	};
}

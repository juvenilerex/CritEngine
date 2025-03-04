#pragma once

#include "RendererAPI.h"

namespace Engine {

	class RenderCommand
	{
	public:
		ENGINE_API inline static void SetClearColor(const glm::vec4& color)
		{
			RenderCommand::rendererAPI->SetClearColor(color);
		}

		ENGINE_API inline static void Clear()
		{
			RenderCommand::rendererAPI->Clear();
		}

		ENGINE_API inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			RenderCommand::rendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static std::unique_ptr<RendererAPI> rendererAPI;
	};

}
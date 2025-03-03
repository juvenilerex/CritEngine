#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Engine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray ) = 0;

		inline static API GetAPI() { return RendererAPI::api; }
		static std::unique_ptr<RendererAPI> Create();

	private:
		static API api;
	};

}
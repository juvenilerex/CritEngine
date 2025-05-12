#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"

namespace Engine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		ENGINE_API static std::unique_ptr<RendererAPI> Create();

		inline static API GetAPI() { return RendererAPI::api; }

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetViewportSize(const int width, const int height) = 0;

		virtual void DrawIndexed(const std::shared_ptr<Mesh>& vertexArray ) = 0;

	private:
		static API api;
	};

}
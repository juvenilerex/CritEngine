#include "Buffer.h"
#include "Renderer.h"

#include "OpenGL/OpenGLBuffer.h"

namespace Engine {
	std::shared_ptr <VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (GraphicsRenderer::GetAPI())
		{
			case RendererAPI::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		ASSERT(false, "Invalid Render API!"); return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (GraphicsRenderer::GetAPI())
		{
			case RendererAPI::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		ASSERT(false, "Invalid Render API!"); return nullptr;
	}
}

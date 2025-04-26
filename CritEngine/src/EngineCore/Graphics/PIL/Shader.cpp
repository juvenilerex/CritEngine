#include "Shader.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace Engine {

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}

}
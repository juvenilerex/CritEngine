#include "Renderer.h" 

namespace Engine {

	void Renderer::BeginScene(std::shared_ptr<PerspectiveCamera> camera)
	{
		Scene::GetActiveScene()->viewProjectionMatrix = camera->GetViewMatrix();
		Scene::GetActiveScene()->perspectiveProjectionMatrix = camera->GetPerspectiveMatrix();
		Scene::GetActiveScene()->viewPerspectiveProjectionMatrix = camera->GetViewPerspectiveMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("uViewProjection", Scene::GetActiveScene()->viewProjectionMatrix);
		shader->UploadUniformMat4("uPerspectiveProjection", Scene::GetActiveScene()->perspectiveProjectionMatrix);
		shader->UploadUniformMat4("uViewPerspectiveProjection", Scene::GetActiveScene()->viewPerspectiveProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
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

	void Renderer::Submit(const std::shared_ptr<Pipeline>& program, const std::shared_ptr<VertexArray>& vertexArray)
	{
		program->Bind();
		program->UploadUniformMat4("uViewProjection", Scene::GetActiveScene()->viewProjectionMatrix);
		program->UploadUniformMat4("uPerspectiveProjection", Scene::GetActiveScene()->perspectiveProjectionMatrix);
		program->UploadUniformMat4("uViewPerspectiveProjection", Scene::GetActiveScene()->viewPerspectiveProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
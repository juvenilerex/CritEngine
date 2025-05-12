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

	void Renderer::Submit(const std::shared_ptr<Model>& model)
	{
		std::shared_ptr<Mesh> mesh = model->GetMesh();
		std::shared_ptr<Pipeline> program = InternalMaterialAccessor::GetPipeline(mesh->GetMaterial());
		program->Bind();
		program->UploadUniformMat4("uViewProjection", Scene::GetActiveScene()->viewProjectionMatrix);
		program->UploadUniformMat4("uPerspectiveProjection", Scene::GetActiveScene()->perspectiveProjectionMatrix);
		program->UploadUniformMat4("uViewPerspectiveProjection", Scene::GetActiveScene()->viewPerspectiveProjectionMatrix);
		program->UploadUniformMat4("uModelProjection", model->GetModelProjection());

		std::vector<std::shared_ptr<Texture>> textures = InternalMaterialAccessor::GetTextures(mesh->GetMaterial());
		for (int i = 0; i < textures.size(); i++)
		{
			textures[i]->Bind(i);
		}

		InternalMeshAccessor::Bind(mesh);
		RenderCommand::DrawIndexed(mesh);
	}
}
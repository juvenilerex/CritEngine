#include <cstdio>
#include <functional>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

#include <EngineCore/Layer.h>
#include <EngineCore/Application.h>
#include <EngineCore/Core/GlobalEngine.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Event/KeyboardEvent.h>
#include <EngineCore/Input/Input.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Math/Vector2.h>
#include <EngineCore/Graphics/Renderer.h>
#include <EngineCore/Graphics/Scene.h>
#include <EngineCore/Graphics/Camera.h>
#include <EngineCore/Graphics/PIL/Texture.h>
#include <EngineCore/Graphics/PIL/Pipeline.h>
#include <EngineCore/Resource/Resource.h>
#include <EngineCore/ECS/Scene.h>
#include <EngineCore/ECS/Components/Spatial.h>
#include <EngineCore/Profiler/Profiler.h>

#include <EngineCore/Tasks/TaskScheduler.h>
#include <EngineCore/Threading/ThreadingHelpers.h>
#include <EngineCore/Resource/Loaders/GLSLShaderLoader.h>

#include <imgui.h>
#include <EngineCore/Graphics/Material.h>
#include <EngineCore/Graphics/Model.h>

const std::filesystem::path ROOT_ASSET_PATH = ((std::filesystem::path)(__FILE__)).parent_path() / "Assets"; // TODO: This is temporary, the engine should provide easy to use "virtual" file system capabilities.

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {
	   
	}

	void OnUpdate() override {
		//LogInfo("ExampleLayer", "Update");
	}

};


class Sandbox : public Engine::Application
{

public:
	// Components will be just data structures that inherit from the Component type. This is an example

	// Systems essentially add underlying behavior to components
	class PhysicsSystem : public ECS::System {
	public:

		void Update() override {
			

			// Perform logic, make modifications, etc..
			const std::chrono::time_point<std::chrono::high_resolution_clock> curTime = std::chrono::high_resolution_clock::now();
			const std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->prevTime);
			for (ECS::EntityID entity : ECS::SceneView<SpatialComponent>(*scene))
			{
				SpatialComponent* primitive = this->scene->GetComponent<SpatialComponent>(entity);
				
				primitive->rotation = primitive->rotation * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, (deltaTime.count() * 10.f), 0));
				primitive->position.y = sinf(std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->startTime).count() * 0.001f);

			}
			this->prevTime = curTime;
		}

		std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();
	};

	PhysicsSystem physicsSystem;
	ECS::Scene componentScene;
	ECS::EntityID player;
  
	void Initialize() override
	{
		CE_PROFILE_FUNC(SandboxInitialization);

		this->window = Engine::GlobalEngine::Get().GetWindowManager().CreateWindow(800, 600, "Sandbox");
		std::shared_ptr<Engine::Window> window = this->window.lock();

		window->GetInput()->OnMouseMove([this](Engine::Vector2(cursorPosition)) { this->MoveCameraLook(cursorPosition); });
		window->GetInput()->OnKeyPressed([this](unsigned int key) { this->MoveCameraPosition(key); });

		this->physicsSystem = PhysicsSystem();
		this->physicsSystem.SetScene(&this->componentScene);

		this->player = this->componentScene.CreateEntity();

		SpatialComponent* primitive = this->componentScene.Assign<SpatialComponent>(player);

		primitive->velocity.x = .0025f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(0.4f, 0.f, 0.f));

		this->camera.reset(new Engine::PerspectiveCamera(30, window->GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, 10), camera_rot));

		Engine::Resource vertexShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.vertshader");
		Engine::Resource fragmentShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.fragshader");

		Engine::Resource textureHandle = Engine::Resource("Image", ROOT_ASSET_PATH / "Textures/Aegis_Jockey.bmp");
		std::shared_ptr<Engine::Texture> sampleTexture = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		std::shared_ptr<Engine::Shader> vertexShader = std::static_pointer_cast<Engine::Shader>(vertexShaderSource.Get());
		std::shared_ptr<Engine::Shader> fragmentShader = std::static_pointer_cast<Engine::Shader>(fragmentShaderSource.Get());

		this->material = Engine::Material::Create(vertexShader, fragmentShader, {sampleTexture});

		//Cube

		Engine::Resource cubeSource = Engine::Resource("Mesh", ROOT_ASSET_PATH / "Meshes/AegisSphere.obj");
		std::shared_ptr<Engine::Mesh> cubeMesh = std::static_pointer_cast<Engine::Mesh>(cubeSource.Get());
		cubeMesh->SetMaterial(this->material);

		this->cubeModel = std::make_shared<Engine::Model>(cubeMesh);

		//Spinny

		float squareVertices[4 * 9] = {
			-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Engine::VertexBuffer> squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float2, "aTexUV"},
			{Engine::ShaderDataType::Float4, "aColor"},
		});

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		std::shared_ptr<Engine::Mesh> squareMesh = Engine::Mesh::Create();
		squareMesh->AddVertexBuffer(squareVB);
		squareMesh->SetIndexBuffer(squareIB);
		squareMesh->SetMaterial(this->material);

		this->squareModel = std::make_shared<Engine::Model>(squareMesh);

		// Floor

		float floorVertices[4 * 9] = {
			-50.0f, 0.0f,-50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 50.0f, 0.0f,-50.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 50.0f, 0.0f, 50.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-50.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Engine::VertexBuffer> floorVB = Engine::VertexBuffer::Create(floorVertices, sizeof(floorVertices));
		floorVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float2, "aTexUV"},
			{Engine::ShaderDataType::Float4, "aColor"},
		});

		uint32_t floorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> floorIB = Engine::IndexBuffer::Create(floorIndices, sizeof(floorIndices) / sizeof(uint32_t));

		std::shared_ptr<Engine::Mesh> floorMesh = Engine::Mesh::Create();
		floorMesh->AddVertexBuffer(floorVB);
		floorMesh->SetIndexBuffer(floorIB);
		floorMesh->SetMaterial(this->material);

		this->floorModel = std::make_shared<Engine::Model>(floorMesh);
		
		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::EnableDepthTest(true);
		Engine::RenderCommand::EnableDepthMask(true);
		Engine::RenderCommand::SetDepthTestFunc(Engine::RendererAPI::DepthTestFunction::Less);
	}

	void MoveCameraLook(Engine::Vector2 cursorPosition)
	{
		if (this->prevCursorPos.x == 0 && this->prevCursorPos.y == 0)
		{
			this->prevCursorPos = cursorPosition;
		}
		Engine::Vector2 velocity = (cursorPosition - this->prevCursorPos) / 1000.f;
		
		// Prevent roll by separating yaw and pitch rotations in the multiplication order.
		this->camera->SetRotation(Engine::Quaternion::FromEulerAngles(Engine::Vector3(velocity.y, 0, 0)) * this->camera->GetRotation() * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, velocity.x, 0)));
		
		this->prevCursorPos = cursorPosition;
		return;
	}

	void MoveCameraPosition(unsigned int key)
	{
		Engine::Vector3 cameraPosition = this->camera->GetPosition();
		Engine::Quaternion cameraRotation = this->camera->GetRotation();
		
		Engine::Vector3 forwardVector = cameraRotation.RotateVector(Engine::Vector3(0, 0, -1));
		Engine::Vector3 rightVector = cameraRotation.RotateVector(Engine::Vector3(1, 0, 0));
		if (key == Keys::KeyMap[Keys::W])
		{
			cameraPosition = cameraPosition + (forwardVector * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::A])
		{
			cameraPosition = cameraPosition + (rightVector * -0.1f);
		}
		if (key == Keys::KeyMap[Keys::S])
		{
			cameraPosition = cameraPosition + (forwardVector * -0.1f);
		}
		if (key == Keys::KeyMap[Keys::D])
		{
			cameraPosition = cameraPosition + (rightVector * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::SPACE])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::LEFT_SHIFT])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * -0.1f);
		}
		
		
		this->camera->SetPosition(cameraPosition);
	}

	void Tick() override
	{	
		// Testing our ECS 
		//CE_PROFILE_FUNC(UpdateLoop);
		std::shared_ptr<Engine::Window> window = this->window.lock();

		this->physicsSystem.Update();
		SpatialComponent* transform = this->componentScene.GetComponent<SpatialComponent>(player);

		this->camera->SetAspectRatio(window->GetAspectRatio());

		//ImGui::Begin("Debug Window");
		//ImGui::Text("TransformComponent Position: %f, %f, %f", transform->position.x, transform->position.y, transform->position.z);
		//ImGui::End();
		this->cubeModel->SetProjection(transform->GetMatrix());

		
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		Engine::Renderer::Submit(this->floorModel);
		//Engine::Renderer::Submit(this->squareModel);
		Engine::Renderer::Submit(this->cubeModel);
		
		Engine::Renderer::EndScene();
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:

	std::weak_ptr<Engine::Window> window;

	std::shared_ptr<Engine::Material> material;
	std::shared_ptr<Engine::Model> squareModel;
	std::shared_ptr<Engine::Model> floorModel;
	std::shared_ptr<Engine::Model> cubeModel;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	Engine::Vector2 prevCursorPos;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
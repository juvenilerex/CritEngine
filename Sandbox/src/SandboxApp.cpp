#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <chrono>

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
#include <EngineCore/ECS/ECSManager.h>
#include <EngineCore/ECS/Components/Spatial.h>
#include <EngineCore/Profiler/Profiler.h>


#include <imgui.h>

std::string vertexShaderSource = R"(
	#version 460 core

	uniform mat4 uViewProjection;
	uniform mat4 uPerspectiveProjection;
	uniform mat4 uModelProjection;
	
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;
	layout (location = 2) in vec2 aTexUV;

	out vec4 vColor;
	out vec2 vTexUV;

	void main()
	{
		
		gl_Position = uPerspectiveProjection * uViewProjection * uModelProjection * vec4(aPos, 1.0);
		vColor = aColor;
		vTexUV = aTexUV;
	}
)";

std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;
	in vec2 vTexUV;

	uniform sampler2D texture1;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = texture(texture1, vTexUV);
	}
)";

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
		// Start with a constructor and pass in a reference to an ECS manager for dependency injection
		PhysicsSystem(Engine::ECSManager& manager) : System(manager) {}

		void Update() override {
			// Get all instances of certain components throughout the manager
			const auto& primatives = manager.GetAllComponents<SpatialComponent>();

			// Perform logic, make modifications, etc..
			const std::chrono::time_point<std::chrono::high_resolution_clock> curTime = std::chrono::high_resolution_clock::now();
			const std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->prevTime);
			for (size_t i = 0; i < primatives.size(); i++) {
				primatives[i]->rotation = primatives[i]->rotation * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, (deltaTime.count() * 10.f), 0));
				
				primatives[i]->position.y = sinf(std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->startTime).count() * 0.001f);
			}
			this->prevTime = curTime;
		}
		std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();
	};

	Engine::ECSManager manager;
	ECS::Entity player;
  
	void Sandbox::Initialize()
	{
		CE_PROFILE_FUNC(SandboxInitialization);

		this->window = Engine::GlobalEngine::Get().GetWindowManager().CreateWindow(800, 600, "Sandbox");
		std::shared_ptr<Engine::Window> window = this->window.lock();

		window->GetInput()->OnMouseMove([this](Engine::Vector2(cursorPosition)) { this->MoveCameraLook(cursorPosition); });
		window->GetInput()->OnKeyPressed([this](unsigned int key) { this->MoveCameraPosition(key); });

		// Register any systems we may want to. All systems will update every frame with UpdateSystems()
		// Return is optional
		manager.RegisterSystem<PhysicsSystem>(manager);

		// Add an Entity to the system. Entities are purely just an ID, and to keep track of them easily, AddEntity()
		// returns the new ID number. Then we'll assign it to an Entity object
		player = manager.AddEntity();

		// We simply add a component to an Entity. The component is automatically created and returned to us when this is called
		auto primitive = manager.AddComponent<SpatialComponent>(player);

		primitive->velocity.x = .0025f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4f, 0.f, 0.f));

		this->camera.reset(new Engine::PerspectiveCamera(30, window->GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -10), camera_rot));

		std::shared_ptr<Engine::Shader> fragmentShader = Engine::Shader::Create(fragmentShaderSource, Engine::ShaderType::Fragment);
		std::shared_ptr<Engine::Shader> vertexShader = Engine::Shader::Create(vertexShaderSource, Engine::ShaderType::Vertex);

		this->program = Engine::Pipeline::Create(vertexShader, fragmentShader);

		//Spinny

		this->squareVA = Engine::VertexArray::Create();

		float squareVertices[4 * 9] = {
			-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
			{Engine::ShaderDataType::Float2, "aTexUV"},
		});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareVA->SetIndexBuffer(squareIB);

		// FLOOR

		this->floorVA = Engine::VertexArray::Create();

		float floorVertices[4 * 9] = {
			-50.0f, 0.0f,-50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 50.0f, 0.0f,-50.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 50.0f, 0.0f, 50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> floorVB = Engine::VertexBuffer::Create(floorVertices, sizeof(floorVertices));
		floorVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
			{Engine::ShaderDataType::Float2, "aTexUV"},
		});
		floorVA->AddVertexBuffer(floorVB);

		uint32_t floorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> floorIB = Engine::IndexBuffer::Create(floorIndices, sizeof(floorIndices) / sizeof(uint32_t));
		floorVA->SetIndexBuffer(floorIB);

		Engine::Resource textureHandle = Engine::Resource("Image", "C:\\Users\\Critical Floof\\Downloads\\bmptestsuite-0.9\\bmptestsuite-0.9\\valid\\Aegis_Jockey_.bmp");

		this->image = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		this->program->Bind();
		this->program->UploadUniformInt("texture1", 0);

	}

	void MoveCameraLook(Engine::Vector2 cursorPosition)
	{
		if (this->prevCursorPos.x == 0 && this->prevCursorPos.y == 0)
		{
			this->prevCursorPos = cursorPosition;
		}
		Engine::Vector2 velocity = (this->prevCursorPos - cursorPosition) / 1000.f;
		
		Engine::Vector3 cameraPosition = this->camera->GetPosition();
		Engine::Quaternion cameraRotation = this->camera->GetRotation();
		Engine::Vector3 forwardVector = cameraRotation.RotateVector(Engine::Vector3(0, 0, -1));

		Debug::Log(forwardVector);
		

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
		Engine::Vector3 upVector = cameraRotation.RotateVector(Engine::Vector3(0, 1, 0));
		Engine::Vector3 rightVector = cameraRotation.RotateVector(Engine::Vector3(1, 0, 0));
		if (key == Keys::KeyMap[Keys::W])
		{
			cameraPosition = cameraPosition + (forwardVector * -0.1);
		}
		if (key == Keys::KeyMap[Keys::A])
		{
			cameraPosition = cameraPosition + (rightVector * -0.1);
		}
		if (key == Keys::KeyMap[Keys::S])
		{
			cameraPosition = cameraPosition + (forwardVector * 0.1);
		}
		if (key == Keys::KeyMap[Keys::D])
		{
			cameraPosition = cameraPosition + (rightVector * 0.1);
		}
		if (key == Keys::KeyMap[Keys::SPACE])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * 0.1);
		}
		if (key == Keys::KeyMap[Keys::LEFT_SHIFT])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * -0.1);
		}
		
		
		this->camera->SetPosition(cameraPosition);
	}

	void Tick() override
	{	
		// Testing our ECS 
		//CE_PROFILE_FUNC(UpdateLoop);
		std::shared_ptr<Engine::Window> window = this->window.lock();

		manager.UpdateSystems();
		auto transform = manager.GetComponent<SpatialComponent>(player);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		this->camera->SetAspectRatio(window->GetAspectRatio());

		//ImGui::Begin("Debug Window");
		//ImGui::Text("TransformComponent Position: %f, %f, %f", transform->position.x, transform->position.y, transform->position.z);
		//ImGui::End();

		Engine::Renderer::BeginScene(this->camera);
		
		

		
		//
		this->image->Bind(0);
		this->program->Bind();
		this->program->UploadUniformMat4("uModelProjection", Engine::Matrix4f::Identity());
		Engine::Renderer::Submit(this->program, this->floorVA);

		this->image->Bind(0);
		this->program->Bind();
		this->program->UploadUniformMat4("uModelProjection", transform->GetMatrix());
		Engine::Renderer::Submit(this->program, this->squareVA);
		

		Engine::Renderer::EndScene();
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:

	std::weak_ptr<Engine::Window> window;

	std::shared_ptr<Engine::Texture> image;

	std::shared_ptr<Engine::Pipeline> program;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<Engine::VertexArray> floorVA;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	Engine::Vector2 prevCursorPos;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
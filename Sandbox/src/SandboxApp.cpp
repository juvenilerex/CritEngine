#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <chrono>

#include <EngineCore/Layer.h>
#include <EngineCore/Application.h>
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
#include <EngineCore/ECS/ECSManager.h>

std::string vertexShaderSource = R"(
	#version 460 core

	uniform mat4 uViewProjection;
	uniform mat4 uPerspectiveProjection;
	uniform mat4 uModelProjection;
	
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	out vec4 vColor;

	void main()
	{
		
		gl_Position = uPerspectiveProjection * uViewProjection * uModelProjection * vec4(aPos, 1.0);
		vColor = aColor;
	}
)";

std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vColor;
	}
)";

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {
	   
	}

	void OnUpdate() override {
//		LogInfo("ExampleLayer", "Update");
	}
	
	void OnEvent(Engine::Event& event) override {

	}

};


class Sandbox : public Engine::Application
{

public:

	// Components will be just data structures that inherit from the Component type. This is an example
	struct Transform : ECS::Component {
		float x, y;
	};

	struct Physics : ECS::Component {
		float velocity = 0;
	};

	// Systems essentially add underlying behavior to components
	class PhysicsSystem : public ECS::System {
	public:
		// Start with a constructor and pass in a reference to an ECS manager for dependency injection
		PhysicsSystem(Engine::ECSManager& manager) : System(manager) {}

		void Update() override {
			// Get all instances of certain components throughout the manager
			const auto& transforms = manager.GetAllComponents<Transform>();
			const auto& physics = manager.GetAllComponents<Physics>();

			// Perform logic, make modifications, etc..
			for (size_t i = 0; i < transforms.size(); i++) {
				transforms[i]->x += physics[i]->velocity;
			}
		}
	};

	Engine::ECSManager manager;
	ECS::Entity player;

	Sandbox() {

		// Register any systems we may want to. All systems will update every frame with UpdateSystems()
		// Return is optional
		manager.RegisterSystem<PhysicsSystem>(manager);

		// Add an Entity to the system. Entities are purely just an ID, and to keep track of them easily, AddEntity()
		// returns the new ID number. Then we'll assign it to an Entity object
		player = manager.AddEntity();

		// We simply add a component to an Entity. The component is automatically created and returned to us when this is called
		auto transform = manager.AddComponent<Transform>(player);

		transform->x = 2.0f;

		auto physics = manager.AddComponent<Physics>(player);

		physics->velocity = 1.5f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4, 0, 0));

		this->camera.reset(new Engine::PerspectiveCamera(90, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -2), camera_rot));

		this->shader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSource));

		this->triangleVA = Engine::VertexArray::Create();

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> triangleVB = Engine::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		triangleVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
							  });
		triangleVA->AddVertexBuffer(triangleVB);

		uint32_t triangleIndices[3] = { 0, 1, 2 };
		std::shared_ptr<Engine::IndexBuffer> triangleIB = Engine::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		triangleVA->SetIndexBuffer(triangleIB);

		this->squareVA = Engine::VertexArray::Create();

		float squareVertices[4 * 7] = {
			-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
							});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareVA->SetIndexBuffer(squareIB);

		this->start = std::chrono::high_resolution_clock::now();
	}

	void OnInputEvent(Engine::Event& event) override {

		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNC(this->TestKeys));
		dispatcher.Dispatch<Engine::MousePressedEvent>(BIND_EVENT_FUNC(this->TestMouse));

	}

	bool TestKeys(Engine::KeyboardEvent& event) {
		event.Print();
		return true; 
	}

	bool TestMouse(Engine::MouseEvent& event) {
		event.Print();
		return true;
	}

	void Tick() override
	{	
		// Testing our ECS 
		manager.UpdateSystems();
		auto transform = manager.GetComponent<Transform>(player);
		Debug::Log("TransformComponent X: ", transform->x);

		//////////////////////

//		LogInfo("Sandbox", "Tick!");

		const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		// TODO: Abstract this behind some generalized object class?
		this->shader->Bind();
		this->shader->UploadUniformMat4("uModelProjection", Engine::Matrix4f({
			cosf(time.count() * 8), 0, sinf(time.count() * 8), 0,
			0, 1, 0, 0,
			-sinf(time.count() * 8), 0, cosf(time.count() * 8), 0,
			0, sinf(time.count() * 4), 0, 1
		}));
		Engine::Renderer::Submit(this->shader, this->squareVA);
		Engine::Renderer::Submit(this->shader, this->triangleVA);

		Engine::Renderer::EndScene();
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}
private:
	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::VertexArray> triangleVA;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
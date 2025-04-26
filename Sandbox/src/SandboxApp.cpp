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
//		LogInfo("ExampleLayer", "Update");
	}
	
	void OnEvent(Engine::Event& event) override {

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
		// I've linked Sandbox with ImGui and this function will ensure that we use the instance inside the application rather than a new one.
		// Without linking we'd have to make a metric ton of wrapper functions, so for now, this is a quick solution.
		ImGui::SetCurrentContext((ImGuiContext*)this->GetImGuiContext());
  
		CE_PROFILE_FUNC(SandboxInitialization);

		// Register any systems we may want to. All systems will update every frame with UpdateSystems()
		// Return is optional
		manager.RegisterSystem<PhysicsSystem>(manager);

		// Add an Entity to the system. Entities are purely just an ID, and to keep track of them easily, AddEntity()
		// returns the new ID number. Then we'll assign it to an Entity object
		player = manager.AddEntity();

		// We simply add a component to an Entity. The component is automatically created and returned to us when this is called
		auto primative = manager.AddComponent<SpatialComponent>(player);

		primative->velocity.x = .0025f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4, 0, 0));

		this->camera.reset(new Engine::PerspectiveCamera(30, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -10), camera_rot));

		this->shader = Engine::Shader::Create(vertexShaderSource, fragmentShaderSource);

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

		Engine::Resource textureHandle = Engine::Resource("Image", "C:\\Users\\Critical Floof\\Downloads\\bmptestsuite-0.9\\bmptestsuite-0.9\\valid\\zenon.bmp");

		this->image = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		this->shader->Bind();
		this->shader->UploadUniformInt("texture1", 0);

	}

	void OnInputEvent(Engine::Event& event) override {

		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNC(this->TestKeys));
		dispatcher.Dispatch<Engine::MousePressedEvent>(BIND_EVENT_FUNC(this->TestMouse));
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNC(this->MoveCamera));

	}

	bool TestKeys(Engine::KeyboardEvent& event) {
		event.Print();
		return true; 
	}

	bool TestMouse(Engine::MouseEvent& event) {
		event.Print();
		return true;
	}

	bool MoveCamera(Engine::MouseMovedEvent& event)
	{
		if (this->prevCursorPos.x == 0 && this->prevCursorPos.y == 0)
		{
			this->prevCursorPos = event.GetCurPos();
		}
		Engine::Vector2 velocity = (this->prevCursorPos - event.GetCurPos()) / 1000.f;
		Debug::Log(velocity);

		// Prevent roll by separating yaw and pitch rotations in the multiplication order.
		this->camera->SetRotation(Engine::Quaternion::FromEulerAngles(Engine::Vector3(velocity.y, 0, 0)) * this->camera->GetRotation() * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, velocity.x, 0)));
		
		this->prevCursorPos = event.GetCurPos();
		return true;
	}

	void Tick() override
	{	
		// Testing our ECS 
		CE_PROFILE_FUNC(UpdateLoop);

		manager.UpdateSystems();
		auto transform = manager.GetComponent<SpatialComponent>(player);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		this->camera->SetAspectRatio(this->GetWindow().GetAspectRatio());

		ImGui::Begin("Debug Window");
		ImGui::Text("TransformComponent Position: %f, %f, %f", transform->position.x, transform->position.y, transform->position.z);
		ImGui::End();

		Engine::Renderer::BeginScene(this->camera);
		
		// TODO: Abstract this behind some generalized object class?
		this->shader->Bind();
		this->shader->UploadUniformMat4("uModelProjection", transform->GetMatrix());

		this->image->Bind(0);
		Engine::Renderer::Submit(this->shader, this->squareVA);

		Engine::Renderer::EndScene();
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:

	std::shared_ptr<Engine::Texture> image;

	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	Engine::Vector2 prevCursorPos;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
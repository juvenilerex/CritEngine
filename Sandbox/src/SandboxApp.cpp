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
#include <EngineCore/Graphics/Texture.h>
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
			for (size_t i = 0; i < primatives.size(); i++) {
				primatives[i]->position = primatives[i]->position + primatives[i]->velocity;
			}
		}
	};

	Engine::ECSManager manager;
	ECS::Entity player;
  
	void Sandbox::Initialize()
	{
		// I've linked Sandbox with ImGui and this function will ensure that we use the instance inside the application rather than a new one.
		// Without linking we'd have to make a metric ton of wrapper functions, so for now, this is a quick solution.
		ImGui::SetCurrentContext((ImGuiContext*)this->GetImGuiContext());
  
		// Register any systems we may want to. All systems will update every frame with UpdateSystems()
		// Return is optional
		CE_PROFILE_MANUAL("Sandbox Initialization: ");
		manager.RegisterSystem<PhysicsSystem>(manager);

		// Add an Entity to the system. Entities are purely just an ID, and to keep track of them easily, AddEntity()
		// returns the new ID number. Then we'll assign it to an Entity object
		player = manager.AddEntity();

		// We simply add a component to an Entity. The component is automatically created and returned to us when this is called
		auto primative = manager.AddComponent<SpatialComponent>(player);

		primative->position.x = 2.0f;
		primative->velocity.x = .025f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		this->start = std::chrono::high_resolution_clock::now();

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4, 0, 0));

		this->camera.reset(new Engine::PerspectiveCamera(90, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -10), camera_rot));

		this->shader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSource));

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

		Engine::Resource textureHandle = Engine::Resource("Image", "C:\\Users\\steve\\Downloads\\bmptestsuite-0.9\\valid\\32bpp-1x1.bmp");

		this->image = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		this->shader->Bind();
		this->shader->UploadUniformInt("texture1", 0);

		CE_PROFILE_MANUAL_STOP;

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

	// For simplicity-sake and full control, we can make direct ImGui calls from here to design UI elements and do whatever
	void OnGUIUpdate() override {
		ImGui::Begin("Debug Window");
		ImGui::Text("Hello, ImGui!");
		ImGui::End();
	}

	void Tick() override
	{	
		// Testing our ECS 
		CE_PROFILE_FUNC("Update Loop");

		manager.UpdateSystems();
		auto transform = manager.GetComponent<SpatialComponent>(player);
		Debug::Log("TransformComponent Position: ", transform->position);

		const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		this->camera->SetAspectRatio(this->GetWindow().GetAspectRatio());


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
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
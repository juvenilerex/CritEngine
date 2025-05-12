#pragma once

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
#include <EngineCore/ECS/ECSManager.h>
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
		// Example API use of the scheduler:
		using Engine::TaskScheduler;
		using Engine::Task;

		TaskScheduler _scheduler;
		_scheduler.Submit(Task([]() {
			// Do stuff here
			Debug::Log("Task ran");
			}));

		// Optionally wait. This blocks the calling thread until the internal task list is empty
		_scheduler.Wait();

		// end example use

		// Batched submit example demonstrating basic fork and join parallelism 
		const size_t numMultiplications = 100000;
		const unsigned int numThreads = std::max(1u, GetThreadCountEstimate());

		Debug::Log("Total multiplications: ", numMultiplications);
		Debug::Log("Number of worker threads: ", numThreads);

		std::vector<int> a(numMultiplications);
		std::vector<int> b(numMultiplications);
		std::vector<int64_t> results(numMultiplications);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1, 100);

		Debug::Log("Generating input data...");
		for (size_t i = 0; i < numMultiplications; ++i) {
			a[i] = distrib(gen);
			b[i] = distrib(gen);
		}

		Debug::Log("Creating Task Scheduler...");
		TaskScheduler scheduler(numThreads);

		Debug::Log("Submitting batched multiplication tasks using the helper...");
		CE_PROFILE_MANUAL(multiThread_BatchedHelper); 

		scheduler.SubmitBatched(numMultiplications, [&](size_t i) 
			{
				int64_t result = static_cast<int64_t>(a[i] * b[i]);
				results[i] = result;
			}
			// Optionally provide desiredTasks hint. e.g: numThreads * 4
			// Default behavior currently adds 4 tasks per every available worker thread
		);

		Debug::Log("All batched tasks submitted.");
		Debug::Log("Waiting for tasks to complete...");

		scheduler.Wait();

		Debug::Log("Tasks completed.");

		CE_PROFILE_MANUAL_STOP(multiThread_BatchedHelper);

		Debug::Log("Summing results...");
		int64_t finalSum = std::accumulate(results.begin(), results.end(), 0LL);
		Debug::Log("Total calculated sum: ", finalSum);

		Debug::Log("Performing sequential verification...");

		CE_PROFILE_MANUAL(singleThread);

		int64_t verificationSum = 0;
		for (size_t i = 0; i < numMultiplications; ++i) {
			verificationSum += static_cast<int64_t>(a[i]) * b[i];
		}

		CE_PROFILE_MANUAL_STOP(singleThread);

		Debug::Log("Verification sum: ", verificationSum);
		ASSERT(finalSum == verificationSum, "Parallel sum does not match sequential sum");
		if (finalSum == verificationSum) {
			Debug::Log("Verification Successful!");
		}
  
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

		Engine::Resource vertexShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.vertshader");
		Engine::Resource fragmentShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.fragshader");

		Engine::Resource textureHandle = Engine::Resource("Image", ROOT_ASSET_PATH / "Textures/Aegis_Jockey.bmp");
		std::shared_ptr<Engine::Texture> sampleTexture = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		std::shared_ptr<Engine::Shader> vertexShader = std::static_pointer_cast<Engine::Shader>(vertexShaderSource.Get());
		std::shared_ptr<Engine::Shader> fragmentShader = std::static_pointer_cast<Engine::Shader>(fragmentShaderSource.Get());

		this->material = Engine::Material::Create(vertexShader, fragmentShader, {sampleTexture});

		//Spinny

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

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		std::shared_ptr<Engine::Mesh> squareMesh = Engine::Mesh::Create();
		squareMesh->AddVertexBuffer(squareVB);
		squareMesh->SetIndexBuffer(squareIB);
		squareMesh->SetMaterial(this->material);

		this->squareModel = std::make_shared<Engine::Model>(squareMesh);

		// Floor

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

		uint32_t floorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> floorIB = Engine::IndexBuffer::Create(floorIndices, sizeof(floorIndices) / sizeof(uint32_t));

		std::shared_ptr<Engine::Mesh> floorMesh = Engine::Mesh::Create();
		floorMesh->AddVertexBuffer(floorVB);
		floorMesh->SetIndexBuffer(floorIB);
		floorMesh->SetMaterial(this->material);

		this->floorModel = std::make_shared<Engine::Model>(floorMesh);
		
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
		this->squareModel->SetProjection(transform->GetMatrix());

		Engine::Renderer::BeginScene(this->camera);

		Engine::Renderer::Submit(this->floorModel);
		Engine::Renderer::Submit(this->squareModel);
		
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
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	Engine::Vector2 prevCursorPos;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
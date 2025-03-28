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
#include <EngineCore/Window/Input.h>
#include <EngineCore/Window/InputMouse.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Math/Vector2.h>
#include <EngineCore/Graphics/Renderer.h>
#include <EngineCore/Graphics/Scene.h>
#include <EngineCore/Graphics/Camera.h>
#include <EngineCore/Resource/Resources/Image.h>

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
		LogInfo("ExampleLayer", "Update");
	}

};

class Sandbox : public Engine::Application
{

public:

	void Sandbox::Initialize()
	{
		PushLayer(new LayerTest());

		this->start = std::chrono::high_resolution_clock::now();

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4, 0, 0));

		this->camera.reset(new Engine::PerspectiveCamera(90, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -2), camera_rot));

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

		std::string filepath = "C:\\Users\\Critical Floof\\Downloads\\bmptestsuite-0.9\\bmptestsuite-0.9\\valid\\555-1x1.bmp"; 
		this->image = std::static_pointer_cast<Engine::Image>(Engine::GlobalEngine::Get().GetResourceManager().GetFromPath("Image", filepath));

		this->shader->Bind();
		this->shader->UploadUniformInt("texture1", 0);
	}

	void Tick() override
	{
		if (this->GetWindow().GetInput().GetKeyJustPressed(Engine::GetKeyCode(Keys::A))) 
		{ // If A is pressed
			LogWarning("Input: ", "A was just pressed");
		}
		if (this->GetWindow().GetMouseInput().GetMouseJustPressed(Engine::GetMouseButton(Mouse::BUTTON_1))) {
			LogWarning("Mouse: ", "Mouse 1 pressed!");
		}

		//LogInfo("Sandbox", "Tick!");

		const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		// TODO: Abstract this behind some generalized object class?
		this->shader->Bind();
		this->shader->UploadUniformMat4("uModelProjection", Engine::Matrix4f({
			cosf(time.count() * 1.5), 0, sinf(time.count() * 1.5), 0,
			0, 1, 0, 0,
			-sinf(time.count() * 1.5), 0, cosf(time.count() * 1.5), 0,
			0, sinf(time.count() * 0.5), 0, 1
		}));

		this->image->GetTexture()->Bind(0);
		Engine::Renderer::Submit(this->shader, this->squareVA);

		Engine::Renderer::EndScene();
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}
private:

	std::shared_ptr<Engine::Image> image;

	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};
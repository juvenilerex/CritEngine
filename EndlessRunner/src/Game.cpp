#pragma once

#include "Game.h"

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

// Red colored shader (because red is danger and obstacle = bad)
std::string fragmentShaderSourceRed = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
)";


class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {

	}

	void OnUpdate() override {
//		LogInfo("ExampleLayer", "Update");
	}

};

class Game : public Engine::Application
{

public:

	Game() {
		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		// Scene initialization logic
		playerBox = std::make_unique<AABB>(Engine::Vector2(0, 0), PLAYER_SIZE);
		player = std::make_unique<Player>();
		spawner = std::make_unique<ObjectSpawner>();
		testBox = std::make_unique<AABB>(Engine::Vector2(-2, 0), Engine::Vector2(0.5f,2.0f));
		rng = std::make_unique<RNG>();

		this->playerBox->size = PLAYER_SIZE;

		this->spawner->Instantiate(obstacles);
		this->spawner->Instantiate(obstacles);


		for (auto& ob : *this->spawner->GetObjects()) {
			LogInfo("Spawner", "Obstacle found! " + std::to_string(ob->transform.position.x));
		}

		/////

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4f, 0, 0));
		this->camera.reset(new Engine::PerspectiveCamera(90, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 3, -9), camera_rot));
		this->shader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSource));
		this->obstacleShader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSourceRed));

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

	std::unique_ptr<Player> player;
	std::unique_ptr<ObjectSpawner> spawner;
	std::shared_ptr<Obstacle> obstacles;
	std::unique_ptr<RNG> rng;

	const float SPEED = 200.0f;
	const float JUMP_VELOCITY = 23.0f;
	const float GRAVITY = 35.0f;
	const float FLOOR_LEVEL = 0.0f;
	const float OBSTACLE_END_POINT = -10.0f;
	const float OBSTACLE_SPEED = 15.0F;
	const float MIN_OBSTACLE_SPAWN_TIME = 1.0f;
	const float MAX_OBSTACLE_SPAWN_TIME = 2.0f;
	const float MIN_OBSTACLE_HEIGHT = 1.0f;
	const float MAX_OBSTACLE_HEIGHT = 3.0f;

	const Engine::Vector2 PLAYER_SIZE = Engine::Vector2(1.0f, 4.0f);

	std::unique_ptr<AABB> playerBox;
	std::unique_ptr<AABB> testBox;

	void Jump() const {
		if (this->player->isGrounded) 
			this->player->velocity.y = JUMP_VELOCITY;
			this->player->isGrounded = false;
	}

	void ApplyGravity() const {
		if (!this->player->isGrounded) 
			this->player->velocity.y -= GRAVITY * Time::deltaTime();		
	}

	float obTimer = 0.0f;
	float timeSinceSpawn = 0.0f;

	void RandomObstacleSpawn() {
		timeSinceSpawn += Time::deltaTime();

		if (timeSinceSpawn >= obTimer) {
			obTimer = this->rng->GetRandFloat(MIN_OBSTACLE_SPAWN_TIME, MAX_OBSTACLE_SPAWN_TIME);
			timeSinceSpawn = 0.0f;
			this->spawner->Instantiate(obstacles);

			// Initializing the object that was just created in the spawner
			auto& ob = std::dynamic_pointer_cast<Obstacle>(this->spawner->GetLastObject());
			ob->box->position = Engine::Vector2(10.0f,0.0f);
			ob->box->size = Engine::Vector2(1.0f, this->rng->GetRandFloat(MIN_OBSTACLE_HEIGHT, MAX_OBSTACLE_HEIGHT));
			
		}
	}

	void Tick() override {
		Time::Update();
		float deltaTime = Time::deltaTime();

		RandomObstacleSpawn();

		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::D))) 
			this->player->velocity.x = SPEED * deltaTime;
		else if (this->GetWindow().GetInput().GetKeyUp(Engine::GetKeyCode(Keys::D))) 
			this->player->velocity.x = 0;
		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::A))) 
			this->player->velocity.x = -SPEED * deltaTime;		
		else if (this->GetWindow().GetInput().GetKeyUp(Engine::GetKeyCode(Keys::A))) 
			this->player->velocity.x = 0;

		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::SPACE)))
			Jump();

		ApplyGravity();

		this->player->transform.position = this->player->transform.position + this->player->velocity * deltaTime;

		if (this->player->transform.position.y <= FLOOR_LEVEL) {
			this->player->transform.position.y = 0;
			this->player->velocity.y = 0;
			this->player->isGrounded = true;
		}

		// Bounding boxes for collision. Could definitely be a component in the future
		this->playerBox->position = Engine::Vector2(this->player->transform.position.x, this->player->transform.position.y);

		//////////////////

		for (auto& ob : *this->spawner->GetObjects()) {
			auto& typedObject = std::dynamic_pointer_cast<Obstacle>(ob);
			typedObject->box->position.x -= OBSTACLE_SPEED * Time::deltaTime();
		}

		// Collision checking

		if (this->playerBox->isColliding(*this->testBox)) {
			LogInfo("AABB", "Colliding!");
		}
		for (auto& ob : *this->spawner->GetObjects()) {
			auto& typedObject = std::dynamic_pointer_cast<Obstacle>(ob);
			if (this->playerBox->isColliding(*typedObject->box)) {
				LogError("AABB", "Player hit!");
			}
		}


		//////////////////////////

		Draw();
	}

	void Draw() {

		const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		this->shader->Bind();


		// Resembling the player with a square
		Engine::Matrix4f modelMatrix = {
			this->playerBox->size.x / 2, 0.0f, 0.0f, 0,
			0.0f, this->playerBox->size.y / 2, 0.0f, 0,
			0.0f, 0.0f, 0.0f, 0.0f,
			this->playerBox->position.x, this->playerBox->position.y, 0.0f, 1
		};

		this->shader->UploadUniformMat4("uModelProjection", modelMatrix);
		Engine::Renderer::Submit(this->shader, this->squareVA);

		Engine::Matrix4f testmodelMatrix = {
			this->testBox->size.x / 2, 0.0f, 0.0f, 0,
			0.0f, this->testBox->size.y / 2, 0.0f, 0,
			0.0f, 0.0f, 0.0f, 0.0f,
			this->testBox->position.x, this->testBox->position.y, 0.0f, 1
		};

		this->shader->UploadUniformMat4("uModelProjection", testmodelMatrix);
		Engine::Renderer::Submit(this->shader, this->squareVA);

		this->shader->Unbind();
		this->obstacleShader->Bind();

		// Resembling obstacles with squares
		for (auto& ob : *this->spawner->GetObjects()) {
			auto& typedObject = std::dynamic_pointer_cast<Obstacle>(ob);
			Engine::Matrix4f m = {
				typedObject->box->size.x / 2, 0.0f, 0.0f, 0,
				0.0f, typedObject->box->size.y / 2, 0.0f, 0,
				0.0f, 0.0f, 1.0f, 0.0f,
				typedObject->box->position.x, typedObject->box->position.y, 0.0f, 1
			};
			this->obstacleShader->UploadUniformMat4("uModelProjection", m);
			Engine::Renderer::Submit(this->obstacleShader, this->squareVA);
		}

		/*

		this->obstacleShader->Unbind();
		this->shader->Bind();

		// Draw the triangle (if needed)
		this->shader->UploadUniformMat4("uModelProjection", Engine::Matrix4f({
			cosf(time.count() * 8), 0, sinf(time.count() * 8), 0,
			0, 1, 0, 0,
			-sinf(time.count() * 8), 0, cosf(time.count() * 8), 0,
			0, sinf(time.count() * 4), 0, 1
			}));
		Engine::Renderer::Submit(this->shader, this->triangleVA);

		*/

		Engine::Renderer::EndScene();

		// Cleaning up obstacles once they enter a certain range
		for (auto& ob : *this->spawner->GetObjects()) {
			if (!ob) {
				break;
			}
			if (ob->transform.position.x <= OBSTACLE_END_POINT) {
				this->spawner->DeleteObject(ob);
				LogInfo("Spawner", "Object deleted!");
			}
		}

	}


	~Game()
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:
	std::shared_ptr<Engine::Shader> obstacleShader;
	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::VertexArray> triangleVA;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Game>();
};

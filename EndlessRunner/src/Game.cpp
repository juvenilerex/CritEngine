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

	std::unique_ptr<Engine::Matrix4f> playerMatrix = std::make_unique<Engine::Matrix4f>();
	std::unique_ptr<Engine::Matrix4f> obstacleMatrix = std::make_unique<Engine::Matrix4f>();

	Game() {
		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());
		
		// Scene initialization logic
		playerBox = std::make_unique<AABB>(Engine::Vector2(0, 0), PLAYER_SIZE);
		player = std::make_unique<Player>();
		spawner = std::make_unique<ObjectSpawner>();
		rng = std::make_unique<RNG>();

		SetMatrixPosition(*playerMatrix, Engine::Vector3(0.0f, 0.0f, 0.0f));
		SetMatrixSize(*playerMatrix, Engine::Vector3(1.0f, 3.0f, 0.0f));

		SetMatrixPosition(*obstacleMatrix, Engine::Vector3(0.0f, 0.0f, 0.0f));
		SetMatrixSize(*obstacleMatrix, Engine::Vector3(1.0f, 3.0f, 0.0f));

		this->playerBox->size = PLAYER_SIZE;

		this->spawner->Instantiate(obstacles);
		this->spawner->Instantiate(obstacles);


		for (auto& ob : *this->spawner->GetObjects()) {
			LogInfo("Spawner", "Obstacle found! " + std::to_string(ob->transform.position.x));
		}

		/////

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(-0.4f, 0, 0));
		this->camera.reset(new Engine::PerspectiveCamera(20, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 20, -60), camera_rot));
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
	const float GRAVITY = 49.0f;
	const float FLOOR_LEVEL = 0.0f;
	const float OBSTACLE_END_POINT = -12.0f;
	const float OBSTACLE_SPEED = 17.0F;
	const float MIN_OBSTACLE_SPAWN_TIME = 0.9f;
	const float MAX_OBSTACLE_SPAWN_TIME = 2.6f;
	const float MIN_OBSTACLE_HEIGHT = 3.0f;
	const float MAX_OBSTACLE_HEIGHT = 8.5f;

	int successfulJumps = 0;

	const Engine::Vector2 PLAYER_SIZE = Engine::Vector2(1.0f, 4.0f);

	std::unique_ptr<AABB> playerBox;

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
			float randHeight = this->rng->GetRandFloat(MIN_OBSTACLE_HEIGHT, MAX_OBSTACLE_HEIGHT);
			const auto& ob = std::dynamic_pointer_cast<Obstacle>(this->spawner->GetLastObject());
			ob->box->size = Engine::Vector2(1.0f, randHeight / 2);
			ob->box->position = Engine::Vector2(15.0f, -1.33f + randHeight / 4);
			ob->transform.position.x = ob->box->position.x;
			ob->transform.position.y = ob->box->position.y;
			ob->transform.scale = Engine::Vector3(ob->box->size.x, ob->box->size.y, 1.0f);
		}
	}
	
	void MovePlayer() {
		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::D)))
			this->player->velocity.x = SPEED * Time::deltaTime();
		else if (this->GetWindow().GetInput().GetKeyUp(Engine::GetKeyCode(Keys::D)))
			this->player->velocity.x = 0;
		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::A)))
			this->player->velocity.x = -SPEED * Time::deltaTime();
		else if (this->GetWindow().GetInput().GetKeyUp(Engine::GetKeyCode(Keys::A)))
			this->player->velocity.x = 0;
	}

	void Tick() override {
		Time::Update();
		float deltaTime = Time::deltaTime();

		RandomObstacleSpawn();
		MovePlayer();

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
			ob->transform.position.x -= OBSTACLE_SPEED * Time::deltaTime();

			// Bounding box, for now, will just be directly copying the transform's position to itself
			const auto& typedObject = std::dynamic_pointer_cast<Obstacle>(ob);
			typedObject->box->position.x = ob->transform.position.x;
			typedObject->box->position.y = ob->transform.position.y;

			if (this->playerBox->isColliding(*typedObject->box)) {
				LogError("AABB", "Player hit!");
			}
		}

		Draw();
	}

	void Draw() {

		const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		this->shader->Bind();

		SetMatrixPosition(*playerMatrix, Engine::Vector3(this->playerBox->position.x, this->playerBox->position.y, 0.0f));
		this->shader->UploadUniformMat4("uModelProjection", *playerMatrix);
		Engine::Renderer::Submit(this->shader, this->squareVA);

		this->shader->Unbind();
		this->obstacleShader->Bind();

		// Resembling obstacles
		for (auto& ob : *this->spawner->GetObjects()) {
			Engine::Matrix4f m = {
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
			SetMatrixPosition(m, ob->transform.position);
			SetMatrixSize(m, ob->transform.scale);
			this->obstacleShader->UploadUniformMat4("uModelProjection", m);
			Engine::Renderer::Submit(this->obstacleShader, this->squareVA);
		}

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

	void SetMatrixPosition(Engine::Matrix4f& mat, const Engine::Vector3& to) {
		mat.data[12] = to.x;
		mat.data[13] = to.y;
		mat.data[14] = to.z;
		mat.data[15] = 1.0f;
	}

	void SetMatrixSize(Engine::Matrix4f& mat, const Engine::Vector3& to) {
		mat.data[0] = to.x / 2;
		mat.data[5] = to.y / 2;
		mat.data[10] = to.z / 2;
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

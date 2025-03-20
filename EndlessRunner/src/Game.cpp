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

std::string fragmentShaderSourceRed = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
)";

std::string fragmentShaderSourceGreen = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
)";

OrthographicCamera::OrthographicCamera(
		float height,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Engine::Vector3 position,
	    Engine::Quaternion rotation
	) 
	    : height(height), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), position(position), rotation(rotation)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

    Engine::Matrix4f OrthographicCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Engine::Matrix4f OrthographicCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Engine::Matrix4f OrthographicCamera::GetViewMatrix() { return this->viewMatrix; }

	void OrthographicCamera::CalculateViewMatrix()
	{
		Engine::Quaternion q = this->rotation;
		Engine::Vector3 p = this->position;

		// In the future the view matrix should be created in place to save on some cycles for every recalculation.

		Engine::Matrix4f rotation = Engine::Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			0, 0, 0, 1
		});

		Engine::Matrix4f position = Engine::Matrix4f({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-p.x, -p.y, -p.z, 1
		});

		this->viewMatrix = rotation * position;

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}

	void OrthographicCamera::CalculatePerspectiveMatrix()
	{
		
		float far = this->farPlane;
		float near = this->nearPlane;
		float top = this->height / 2;
		float bottom = -this->height / 2;
		float left = this->height * this->aspectRatio / 2;
		float right = -this->height * this->aspectRatio / 2;

		this->perspectiveMatrix = Engine::Matrix4f({
			-2 / (right - left), 0, 0, 0,
			0, 2 / (top - bottom), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1,
		});

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
		
	}

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {

	}

	void OnUpdate() override {
		LogInfo("ExampleLayer", "Update");
	}

};

class EndlessRunner : public Engine::Application
{

public:

	std::unique_ptr<Player2D> player;
	std::unique_ptr<ObjectSpawner> spawner;
	std::shared_ptr<Obstacle> obstacles;
	std::shared_ptr<Collider2D> passFlag;
	std::unique_ptr<RandomNumberGenerator> rng;

	EndlessRunner() {
		PushLayer(new LayerTest());

		Engine::Scene::SetActiveScene(std::make_shared<Engine::Scene>());

		// Game initialization

		player = std::make_unique<Player2D>(Engine::Vector2(1.0f, 1.0f), Engine::Vector2(3.0f, 7.0f));
		playerDrawQueue.push_back(Engine::Matrix4f::Identity());

		spawner = std::make_unique<ObjectSpawner>();
		rng = std::make_unique<RandomNumberGenerator>();

		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, 0, 0));

		this->camera.reset(new OrthographicCamera(20, this->GetWindow().GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, -50), camera_rot));

		this->greenShader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSourceGreen));
		this->redShader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSourceRed));
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
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
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

	const float SPEED = 10.0f;
	const float JUMP_HEIGHT = 26.0f;
	const float GRAVITY = 70.0f;
	const float FLOOR_LEVEL = 0.0f;
	const float OBSTACLE_START_POINT = 15.0f;
	const float OBSTACLE_END_POINT = -12.0f;
	const float OBSTACLE_SPEED = 20.0F;
	const float MIN_OBSTACLE_SPAWN_TIME = 0.4f;
	const float MAX_OBSTACLE_SPAWN_TIME = 1.6f;
	const float MIN_OBSTACLE_HEIGHT = 2.0f;
	const float MAX_OBSTACLE_HEIGHT = 4.7f;
	const float LEVEL_BOUNDS_LEFT = -11.88f;
	const float LEVEL_BOUNDS_RIGHT = 11.88f;

	int health = 3;
	int score = 0;

	float obTimer = 0.0f;
	float timeSinceSpawn = 0.0f;

	void RandomObstacleSpawn(const float deltaTime) {
		timeSinceSpawn += deltaTime;

		if (timeSinceSpawn >= obTimer) {
			obTimer = this->rng->GetRandFloat(MIN_OBSTACLE_SPAWN_TIME, MAX_OBSTACLE_SPAWN_TIME);
			timeSinceSpawn = 0.0f;
			this->spawner->Instantiate(obstacles);

			float randHeight = this->rng->GetRandFloat(MIN_OBSTACLE_HEIGHT, MAX_OBSTACLE_HEIGHT);
			const auto& ob = std::dynamic_pointer_cast<Obstacle>(this->spawner->GetLastObject());

			ob->transform.position = Engine::Vector2(OBSTACLE_START_POINT, -3.5f + randHeight / 2);
			ob->transform.scale = Engine::Vector2(1.0f, randHeight);

			ob->boundingBox->SetDimensions(ob->transform.scale);
			ob->boundingBox->SetPosition(ob->transform.position);

			int chance = this->rng->GetRandInt(1, 2);
			if (chance == 1) {
				ob->transform.position = Engine::Vector2(ob->transform.position.x, ob->transform.position.y * -1 + 8.0f);
				ob->transform.scale = Engine::Vector2(ob->transform.scale.x, ob->transform.scale.y * 2.35);
				ob->boundingBox->SetPosition(ob->transform.position);
				ob->boundingBox->SetDimensions(ob->transform.scale);
			}

			this->spawner->Instantiate(passFlag);
			const auto& flag = std::dynamic_pointer_cast<Collider2D>(this->spawner->GetLastObject());

			flag->transform.position = Engine::Vector2(ob->transform.position.x, -0.0f);
			flag->transform.scale = Engine::Vector2(1.0f, 20.0f);

			flag->boundingBox->SetDimensions(flag->transform.scale);
			flag->boundingBox->SetPosition(flag->transform.position);


		}
	}

	void MovePlayer(const float deltaTime) {
		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::D))) {
			this->player->velocity.x = SPEED;
		}
		else if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::A))) {
			this->player->velocity.x = -SPEED; 
		}
		else {
			this->player->velocity.x = 0; 
		}
		if (this->GetWindow().GetInput().GetKeyDown(Engine::GetKeyCode(Keys::SPACE))) {
			if (this->player->isGrounded) {
				this->player->velocity.y = JUMP_HEIGHT;
				this->player->isGrounded = false; 
			}
		}
	}

	DeltaTime dT;

	void Tick() override
	{
		float deltaTime = dT.GetDeltaTime();

		RandomObstacleSpawn(deltaTime);
		MovePlayer(deltaTime);

		if (!this->player->isGrounded) {
			this->player->velocity.y -= GRAVITY * deltaTime; 
		}

		this->player->transform.position.x += this->player->velocity.x * deltaTime;
		this->player->transform.position.y += this->player->velocity.y * deltaTime;


		if (this->player->transform.position.x <= LEVEL_BOUNDS_LEFT) {
			this->player->transform.position.x = LEVEL_BOUNDS_LEFT + 0.01f;
		} 
		if (this->player->transform.position.x >= LEVEL_BOUNDS_RIGHT) {
			this->player->transform.position.x = LEVEL_BOUNDS_RIGHT - 0.01f;
		}

		if (this->player->transform.position.y <= FLOOR_LEVEL) {
			this->player->transform.position.y = 0;
			this->player->velocity.y = 0;
			this->player->isGrounded = true;
		}

		
		this->player->boundingBox->SetPosition(this->player->transform.position);
		this->player->boundingBox->SetDimensions(this->player->transform.scale);

		for (const auto& ob : *this->spawner->GetObjects()) {

			if (const auto& typedObject = std::dynamic_pointer_cast<Obstacle>(ob)) {
				typedObject->transform.position.x -= OBSTACLE_SPEED * deltaTime;
				typedObject->boundingBox->SetPosition(typedObject->transform.position);

				if (this->player->boundingBox->isCollidingWith(*typedObject->boundingBox)) {
					this->spawner->DeleteObject(ob);
					health -= 1;
					LogError("AABB", "Player hit! Health: " + std::to_string(health));
					if (health <= 0) {
						health = 3;
						score = 0;
					}
				}
			}
		}

		for (const auto& ob : *this->spawner->GetObjects()) {

			if (const auto& typedObject = std::dynamic_pointer_cast<Collider2D>(ob)) {
				typedObject->transform.position.x -= OBSTACLE_SPEED * deltaTime;
				typedObject->boundingBox->SetPosition(typedObject->transform.position);

				if (this->player->boundingBox->isCollidingWith(*typedObject->boundingBox)) {
					this->spawner->DeleteObject(ob);
					score++;
					LogWarning("AABB", "Player scored: " + std::to_string(score));
				}
			}
		}


		////////////////////////////

		SetMatrixPosition(playerDrawQueue[0], this->player->transform.position);
		SetMatrixSize(playerDrawQueue[0], this->player->transform.scale);

//		SetMatrixPosition(debugBoxDrawQueue[1], this->player->boundingBox->GetPosition());
//		SetMatrixSize(debugBoxDrawQueue[1], this->player->boundingBox->GetDimensions());

		Render();

		for (const auto& ob : *this->spawner->GetObjects()) {
			if (!ob) {
				break;
			}
			if (ob->transform.position.x <= OBSTACLE_END_POINT) {
				this->spawner->DeleteObject(ob);
			}
		}
	}

	enum ObjectID {
		PLAYER = 0, OBSTACLE = 1, DEBUGBOX = 2
	};

	std::vector<Engine::Matrix4f> playerDrawQueue;
	std::vector<Engine::Matrix4f> obstacleDrawQueue;
	std::vector<Engine::Matrix4f> debugBoxDrawQueue;

    void Render() const {
        const std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

        Engine::RenderCommand::SetClearColor({ 0.3, 0.2, 0.8, 1 });
        Engine::RenderCommand::Clear();

        //This is begin scene lmao
        Engine::Scene::GetActiveScene()->viewProjectionMatrix = camera->GetViewMatrix();
        Engine::Scene::GetActiveScene()->perspectiveProjectionMatrix = camera->GetPerspectiveMatrix();
        Engine::Scene::GetActiveScene()->viewPerspectiveProjectionMatrix = camera->GetViewPerspectiveMatrix();
        //

        for (const auto& mat : playerDrawQueue) {
            PrimitiveDraw::Draw(this->shader, mat, this->squareVA);
        }
        for (const auto& mat : debugBoxDrawQueue) {
            PrimitiveDraw::Draw(this->redShader, mat, this->squareVA);
        }

        for (const auto& ob : *this->spawner->GetObjects()) {
			if (const auto& obstacle = std::dynamic_pointer_cast<Obstacle>(ob)) {
				Engine::Matrix4f m = Engine::Matrix4f::Identity();
				SetMatrixPosition(m, ob->transform.position);
				SetMatrixSize(m, ob->transform.scale);
				if(obstacle->transform.scale.y < MAX_OBSTACLE_HEIGHT / 1.5)
					PrimitiveDraw::Draw(this->redShader, m, this->squareVA);
				if (obstacle->transform.scale.y > MAX_OBSTACLE_HEIGHT / 1.5)
					PrimitiveDraw::Draw(this->greenShader, m, this->squareVA);
			}
        }

        Engine::Renderer::EndScene();
    }

    void SetMatrixPosition(Engine::Matrix4f& mat, const Engine::Vector3& to) const {
        mat.data[12] = to.x;
        mat.data[13] = to.y;
        mat.data[14] = to.z;
    }

    void SetMatrixPosition(Engine::Matrix4f& mat, const Engine::Vector2& to) const {
        mat.data[12] = to.x;
        mat.data[13] = to.y;
    }

	void SetMatrixSize(Engine::Matrix4f& mat, const Engine::Vector3& to) const {
		mat.data[0] = to.x / 2;
		mat.data[5] = to.y / 2;
		mat.data[10] = to.z / 2;
	}

	void SetMatrixSize(Engine::Matrix4f& mat, const Engine::Vector2& to) const {
		mat.data[0] = to.x / 2;
		mat.data[5] = to.y / 2;
	}

	~EndlessRunner()
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:
	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::Shader> redShader;
	std::shared_ptr<Engine::Shader> greenShader;
	std::shared_ptr<Engine::VertexArray> triangleVA;
	std::shared_ptr<Engine::VertexArray> squareVA;
	std::shared_ptr<OrthographicCamera> camera;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<EndlessRunner>();
};
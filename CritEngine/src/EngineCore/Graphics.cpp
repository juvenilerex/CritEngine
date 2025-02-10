#include "Graphics.h"
#include "Core/MainLoop.h" 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const std::string vertexShaderSource = R"(
	#version 460 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	out vec4 vColor;

	void main()
	{
		
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		vColor = aColor;
	}
)";

const std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vColor;
	}
)";



namespace Engine {

	GraphicsRenderer::GraphicsRenderer(Engine::Window& window_ref) 
		: windowHandle(window_ref.GetHandle())
	{
		glfwMakeContextCurrent(this->windowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LogError("GLAD", "Failed to initialize GLAD!");
			exit(EXIT_FAILURE);
		}

		glfwSwapInterval(1); // Wait on 1 monitor refresh before swapping buffers / VSync

		// Compiling the shaders
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexCStr = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexCStr, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentCStr = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentCStr, NULL);
		glCompileShader(fragmentShader);

		// Defining a program and attaching the compiled shader instructions to the program
		this->shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Once the program is created, we can tell OpenGL to use it everytime we attempt to draw.
		glUseProgram(this->shaderProgram);

		// Deleting the shaders because we're not composing any new programs.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Now the shader pipeline is built, consisting of a vertex and fragment (pixel) shader.
		// Next step is to prepare the data which the program will use to render, this is effectively the context the program will run in.
		
		float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		// The job of a vertexBufferObject is to just store a bunch of bytes in GPU memory.
		// Create a buffer and bind that buffer to GLArrayBuffer, which creates the buffer in the GPU (because it doesn't exist yet) and makes it the active buffer, 
		// then copy the vertex data into the active buffer.
		unsigned int vertexBufferObject;
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Now that we have an active buffer, we can create the VertexArrayObject by generating a name, and then creating it (because it doesn't exist yet) and making it active, just like the buffer.
		// The job of the VAO is to act as a descriptor for the currently active buffer, effectively informing OpenGL and us on how to read/write the data
		glGenVertexArrays(1, &this->vertexArrayObject);
		glBindVertexArray(this->vertexArrayObject);

		// We then encode how our data in the buffer is structured using the following commands.
		// Enable the attribute at location 0 (which is used for 3D position in the vertex shader we're using)
		glEnableVertexAttribArray(0);
		// Then define a pointer for the shader code to work with,
		// this pointer is representing location 0, 
		// a component count of 3 (as this encodes a 3D position), 
		// the data being read is of type float, 
		// normalization set to false so fixed-point values are NOT constrained to 0.0 to 1.0, though we aren't using integers so this does nothing here, 
		// the stride is 28 bytes long as we're reading 7 different floats for every vertex (position + color in our shader's case),
		// and the offset is 0.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
		
		// Encodes vertex color as defined by location 1 in the vertex shader.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	}

	GraphicsRenderer::~GraphicsRenderer()
	{

	}

	void GraphicsRenderer::Draw()
	{
		glClearColor(0.6f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(windowHandle);
	}

}
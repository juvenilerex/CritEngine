#include "../Graphics.h" 
#include <glad/glad.h>
#include <GLFW/glfw3.h>


std::string vertexShaderSource = R"(
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

std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vColor;
	}
)";



namespace Engine {

	GraphicsRenderer::GraphicsRenderer(GLFWwindow* window_ref)
		: windowHandle(window_ref)
	{
		glfwSwapInterval(1); // Wait on 1 monitor refresh before swapping buffers / VSync

		shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));

		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);

		unsigned int vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		unsigned int indexBuffer;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

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

		this->shader->Bind();
		glBindVertexArray(this->vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

}
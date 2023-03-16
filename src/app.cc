#include "app.h"
#include "window.h"
#include <iostream>

App::App() {

}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void App::Run() {
	glfwInit();

	window_ = new Window("Raytracing!", 1000, 600);
	window_->MakeContextCurrent();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glViewport(0,0, 1000, 600);
	// Create Shaders.
	render_img_shader_ = new Shader{"res/shaders/render_img.vs", "res/shaders/render_img.fs"};
	raytracing_shader_ = new Shader{"res/shaders/raytracing.cs"};

	// Create OpenGL image object.
	glGenTextures(1, &img_id_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, img_id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, kImageResolution.x, kImageResolution.y, 0, GL_RGBA,
		GL_FLOAT, NULL);

	glBindImageTexture(0, img_id_, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, img_id_);
	
	// Create OpenGl quad VAO.
	float quadVertices[] = {
		 1.f,  1.f,
		 1.f, -1.f,
		-1.f, -1.f,
		-1.f,  1.f,
	};
	int quadIndices[] = {
		0, 1, 3,
		1, 2, 3,
	};
	uint32_t quadVAO, quadVBO, indicesVBO;

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), quadIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);



	//NOTE: Probably rendering the quad using indices its stupid idea.
	//		Should use triangle strips 
	//		Doesn't much matter.
	while (!window_->ShouldClose()) {
		raytracing_shader_->Use();
		raytracing_shader_->SetFloat("u_fov", kFOV);
		raytracing_shader_->SetV3f("u_camera_position", {1,0,10});
		glDispatchCompute((unsigned int)kImageResolution.x / 10, (unsigned int)kImageResolution.y / 10, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		render_img_shader_->Use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);
		window_->Update();
	}
}

App::~App() {
	glDeleteTextures(1, &img_id_);
	glfwTerminate();
}
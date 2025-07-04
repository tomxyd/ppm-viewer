//Author: Tolulope Mojid
//Date: 04-07-2-25


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <chrono>

#include "PPMImage.h"
#include "Shader.h"

PPMImage ppmImage;

const int width = 1280;
const int height = 720;

float vertices[] = {
	// positions   // colors       // tex coords
	1.0f,  1.0f, 0.0f,  1.0f,0.0f,0.0f,     1.0f, 1.0f,  // top right
	1.0f, -1.0f, 0.0f,  0.0f,1.0f,0.0f,     1.0f, 0.0f,  // bottom right
   -1.0f, -1.0f, 0.0f,  0.0f,0.0f,1.0f,     0.0f, 0.0f,  // bottom left
   -1.0f,  1.0f, 0.0f,  1.0f,1.0f,0.0f,     0.0f, 1.0f   // top left
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

bool showImage = false;
bool imageLoaded = false;
float loadTimeMs = 0.0f;

void Load(const std::string& filePath)
{
	std::ifstream file(RESOURCES_PATH + filePath);
	if (!file)
		throw std::runtime_error("Could not open file");
	ppmImage.ParseHeaderData(file);
	ppmImage.LoadPixels(file);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	const char* glsl_version = "#version 100";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "PPM Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);

	Shader quadShader(RESOURCES_PATH "vertex.vc", RESOURCES_PATH "fragment.fc");

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	
	//load texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	quadShader.use();
	quadShader.setInt("texture1", 0);


	while (!glfwWindowShouldClose(window))
	{
		// input
		   // -----
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (showImage && imageLoaded) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			quadShader.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("PPM Debug Window");                         

			if (ImGui::Button("Load PPM Image"))
			{
				auto start = std::chrono::high_resolution_clock::now();

				try {
					Load("lola.ppm");
					glBindTexture(GL_TEXTURE_2D, texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ppmImage.h._width, ppmImage.h._height, 0,
						GL_BGRA, GL_UNSIGNED_BYTE, ppmImage.GetPixelBuffer().data());

					imageLoaded = true;
				}
				catch (const std::exception& e) {
					std::cerr << "Failed to load image: " << e.what() << "\n";
					imageLoaded = false;
				}

				auto end = std::chrono::high_resolution_clock::now();
				loadTimeMs = std::chrono::duration<float, std::milli>(end - start).count();
			}

			ImGui::Text("Load time: %.2f ms", loadTimeMs);
			ImGui::Checkbox("Show Image", &showImage);
			float minutes = loadTimeMs / 60000.0f;
			ImGui::Text("Load time in minutes: %.1f", minutes);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
		// render
		// ------
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



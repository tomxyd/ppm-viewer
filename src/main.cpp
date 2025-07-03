#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PPMImage.h"
#include "Shader.h"

PPMImage ppmImage;

void Load(const std::string& filePath)
{
	std::ifstream file(RESOURCES_PATH + filePath);
	if (!file)
		throw std::runtime_error("Could not open file");
	ppmImage.ParseHeaderData(file);
	ppmImage.LoadPixels(file);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	try
	{
		Load("sample.ppm");
	}
	catch (const std::exception& err)
	{
		std::cerr << "Error: " << err.what() << std::endl;
		return -1;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "PPM Viewer", NULL, NULL);
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

	glViewport(0, 0, 800, 600);
	framebuffer_size_callback(window, 800, 600);
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



/*
* Copyright (c) 2022 - The OpenFlight Team
* 
* Main.cpp
*/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// -- SETTINGS --
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const char* TITLE = "OpenFlight";
// -- END SETTINGS --

// -- FORWARD DECLARATIONS --
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
// -- END FORWARD DECLARATIONS --

int main()
{
	// -- SETUP --
	if (!glfwInit())
	{
		// TODO: Replace with custom logger
		std::cerr << "Failed to initialize GLFW. Exiting..." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Abstract window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (window == NULL)
	{
		// TODO: Replace with custom logger
		std::cerr << "Failed to create GLFW window. Exiting..." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD. Exiting..." << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// -- END SETUP --

	// -- MAIN GAME LOOP --
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// -- END MAIN GAME LOOP --

	glfwTerminate();

	return 0;
}

// Function to resize the viewport when the user changes the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Simple process of input, should be handed off to another class for handling later, just temp
// TODO: Hand off to another dedicated input class
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
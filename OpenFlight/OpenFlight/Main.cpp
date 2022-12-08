/*
* Copyright (c) 2022 - The OpenFlight Team
* 
* Main.cpp
*/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO: Switch all std output to custom logger for writing to files and outputting
#include "Logger.h"
#include "Types.h"
#include "Renderer.h"

// -- SETTINGS --
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const char* TITLE = "OpenFlight";
// -- END SETTINGS --

// -- FORWARD DECLARATIONS --
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
// -- END FORWARD DECLARATIONS --

// -- SYSTEMS --
Logger logger;
Renderer mainRenderer;
// -- END SYSTEMS --
	
int main()
{
	// -- SETUP --

	// Logging system setup
	if (!logger.initializeLogging())
	{
		logger.logOut(LOG_LVL_ERR, "Failed to initialize logger. Exiting...");
		return -1;
	}

	if (!glfwInit())
	{
		logger.logOut(LOG_LVL_ERR, "Failed to initialize GLFW. Exiting...");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (!window)
	{
		logger.logOut(LOG_LVL_ERR, "Failed to create GLFW window. Exiting...");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger.logOut(LOG_LVL_ERR, "Failed to initialize GLAD. Exiting...");
		return -1;
	}

	// Initialize renderer
	if (!mainRenderer.init(logger))
	{
		logger.logOut(LOG_LVL_ERR, "Failed to initialize renderer. Exiting...");
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	logger.logOut(LOG_LVL_INFO, "Renderer, OpenGL version, GLSL Version:");

	std::cout << renderer << std::endl;
	std::cout << version << std::endl;
	std::cout << glslVersion << std::endl;

	// -- END SETUP --

	logger.logOut(LOG_LVL_INFO, "Setup succeeded, starting...");

	// -- SETUP GRAPHICS PIPELINE --
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// -- END GRAPHICS PIPELINE SETUP -- 

	mainRenderer.setup(vertices);

	// -- MAIN GAME LOOP --
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		mainRenderer.clearScreen(0.5f, 0.5f, 0.5f, 1.0f);

		mainRenderer.render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	// -- END MAIN GAME LOOP --

	// After the main loop is exited cleanup the logger and close GLFW
	mainRenderer.cleanup();
	logger.cleanup();
	glfwTerminate();

	return 0;
}

// Function to resize the viewport when the user changes the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Simple input processing, should be handed off to another class for handling later, just temp
// TODO: Hand off to another dedicated input class
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
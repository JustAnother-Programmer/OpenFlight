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
	if (window == NULL)
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

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// -- END SETUP --

	logger.logOut(LOG_LVL_INFO, "Setup succeeded, starting...");

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

	logger.cleanup();
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
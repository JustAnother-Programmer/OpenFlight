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

		// -- BEGIN TEMP TESTING --

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex shader source GLSL
		const char* vertexShaderSrc = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\n";

		// Fragment shader source GLSL
		const char* fragmentShaderSrc = "#version 330 core\n"
			"out vec4 fragColour;\n"
			"void main()\n"
			"{\n"
			"fragColour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n";

		// Create vertex shader
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Create fragment shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile the source
		glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShader);

		glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShader);

		// Check for errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		int tempSuccess2;
		char infoLog2[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &tempSuccess2);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			logger.logOut(LOG_LVL_ERR, "Fatal Error: Vertex shader failed to compile. Err: ");
			std::cout << infoLog << std::endl;
		}

		if (!tempSuccess2)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
			logger.logOut(LOG_LVL_ERR, "Fatal Error: Fragment shader failed to compile. Err: ");
			std::cout << infoLog << std::endl;
		}

		// Create shader program
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int success3;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);

		if (!success3) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			logger.logOut(LOG_LVL_ERR, "Fatal Error: Failed to link shaders. Err: ");
			std::cout << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glUseProgram(shaderProgram);

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// -- END TEMP TESTING -- 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// -- END MAIN GAME LOOP --

	// After the main loop is exited cleanup the logger and close GLFW
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
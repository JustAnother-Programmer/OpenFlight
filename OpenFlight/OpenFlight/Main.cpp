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

enum ShaderType
{
	VERTEX,
	FRAGMENT
};

// -- SETTINGS --
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const char* TITLE = "OpenFlight";
// -- END SETTINGS --

// -- FORWARD DECLARATIONS --
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// TODO: Change these to a seperate class such as error checker
bool checkIsSuccessShader(unsigned int program, ShaderType type);
bool checkIsSuccessProgram(unsigned int program);
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	logger.logOut(LOG_LVL_INFO, "Renderer and current version:");

	std::cout << renderer << std::endl;
	std::cout << version << std::endl;

	// -- END SETUP --

	logger.logOut(LOG_LVL_INFO, "Setup succeeded, starting...");

	// -- SETUP GRAPHICS PIPELINE --
	float vertices[] = {
	   0.0f,  0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f
	};

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Vertex shader source GLSL
	const char* vertexShaderSrc =
		"#version 440\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	// Fragment shader source GLSL
	const char* fragmentShaderSrc =
		"#version 440\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	// Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Compile the source
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	if (!checkIsSuccessShader(vertexShader, VERTEX))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured in the vertex shader");
	}

	// Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	if (!checkIsSuccessShader(fragmentShader, FRAGMENT))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured in the fragment shader");
	}

	// Create shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	if (!checkIsSuccessProgram(shaderProgram))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured in the shader program");
	}

	// -- END GRAPHICS PIPELINE SETUP -- 

	// -- MAIN GAME LOOP --
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -- BEGIN TEMP TESTING --

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// -- END TEMP TESTING -- 

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glfwPollEvents();
		glfwSwapBuffers(window);
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

bool checkIsSuccessShader(unsigned int program, ShaderType type)
{
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured during compilation of a shader. See more below.\n");

		switch (type)
		{
		case VERTEX:
		case FRAGMENT:
			glGetShaderInfoLog(program, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
			break;
		default:
			break;
		}

		return false;
	}

	return true;
}

bool checkIsSuccessProgram(unsigned int program)
{
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) 
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;

		return false;
	}

	return true;
}
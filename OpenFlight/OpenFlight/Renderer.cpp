/*
* Copyright (c) 2022 - The OpenFlight Team
*
* Renderer.cpp
*/

#include "Renderer.h"

// TODO: Add shader loader

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

bool Renderer::init(Logger primaryLogger)
{
	logger = primaryLogger;

	return true;
}

void Renderer::cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glCheckError();
	
	glDeleteBuffers(1, &VBO);
	glCheckError();

	glDeleteProgram(shaderProgram);
	glCheckError();
}


void Renderer::setup(float* vertices)
{
	// Create vertex shader
	compileShader(VERTEX, vertexShaderSrc);

	if (!validateShader(vertexShader, VERTEX))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while validating the vertex shader");
	}

	// Create fragment shader
	compileShader(FRAGMENT, fragmentShaderSrc);

	if (!validateShader(fragmentShader, FRAGMENT))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while validating the fragment shader");
	}

	// Create shader program
	compileProgram();

	if (!validateProgram(shaderProgram))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while compiling the shader program");
	}

	generateBuffers(vertices);
}

// This needs a refactor to include the while loop to prevent memory leaks
void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCheckError();

	glUseProgram(shaderProgram);
	glCheckError();

	glBindVertexArray(VAO);
	glCheckError();

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glCheckError();
	
	glBindVertexArray(0);
	glCheckError();
}

void Renderer::clearScreen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glCheckError();
}

void Renderer::compileShader(ShaderType type, const char* src)
{
	switch (type)
	{
	case VERTEX:
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Compile the source
		glShaderSource(vertexShader, 1, &src, NULL);
		glCheckError();
		glCompileShader(vertexShader);
		glCheckError();
		break;
	case FRAGMENT:
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile the source
		glShaderSource(fragmentShader, 1, &src, NULL);
		glCheckError();
		glCompileShader(fragmentShader);
		glCheckError();
		break;
	default:
		logger.logOut(LOG_LVL_ERR, "Fatal Error: Shader type unkown");
		break;
	}
}

void Renderer::compileProgram()
{
	// Create shader program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glCheckError();
	glAttachShader(shaderProgram, fragmentShader);
	glCheckError();
	glLinkProgram(shaderProgram);
	glCheckError();

	// Delete unessecary shaders
	glDeleteShader(vertexShader);
	glCheckError();
	glDeleteShader(fragmentShader);
	glCheckError();
}

void Renderer::generateBuffers(float* vertices)
{
	// Vertex Buffers
	glGenBuffers(1, &VBO);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glCheckError();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckError();

	// Vertex Arrays
	glGenVertexArrays(1, &VAO);
	glCheckError();

	glBindVertexArray(VAO);
	glCheckError();

	glEnableVertexAttribArray(0);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glCheckError();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glCheckError();

	glBindVertexArray(0);
	glCheckError();
}

bool Renderer::validateShader(GLuint shader, ShaderType type)
{
	int success;
	char infoLog[512];

	glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
	glCheckError();

	if (!success)
	{
		switch (type)
		{
		case VERTEX:
		case FRAGMENT:
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			glCheckError();
			std::cout << infoLog << std::endl;
			break;
		default:
			break;
		}

		return false;
	}

	return true;
}

bool Renderer::validateProgram(GLuint program)
{
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glCheckError();

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		glCheckError();
		std::cout << infoLog << std::endl;

		return false;
	}

	return true;
}

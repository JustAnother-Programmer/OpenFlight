/*
* Copyright (c) 2022 - The OpenFlight Team
*
* Renderer.cpp
*/

#include "Renderer.h"

// Vertex shader source GLSL
// TODO: extract to own text files and create a loader
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
	this->logger = primaryLogger;

    return true;
}

void Renderer::cleanup()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteProgram(this->shaderProgram);
}

void Renderer::setup(float vertices[], unsigned int indices[])
{
	// TODO: Directly initalize this stuff to the members of the class rather than other variables
	
	// Create vertex shader
	compileShader(VERTEX, vertexShaderSrc);

	if (!validateShader(this->vertexShader, VERTEX))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while validating the vertex shader");
	}

	// Create fragment shader
	compileShader(FRAGMENT, fragmentShaderSrc);

	if (!validateShader(this->fragmentShader, FRAGMENT))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while validating the fragment shader");
	}

	// Create shader program
	compileProgram();

	if (!validateProgram(this->shaderProgram))
	{
		logger.logOut(LOG_LVL_ERR, "Fatal Error: An error occured while compiling the shader program");
	}

	generateBuffers(vertices);

	generateEBO(indices);
}

// This needs a refactor to include the while loop to prevent memory leaks
void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->VAO);
	glCheckError();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glCheckError();

	glBindVertexArray(0);
}

void Renderer::clearScreen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::compileShader(ShaderType type, const char* src)
{
	switch (type)
	{
	case VERTEX:
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Compile the source
		glShaderSource(this->vertexShader, 1, &src, NULL);
		glCompileShader(this->vertexShader);
		break;
	case FRAGMENT:
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile the source
		glShaderSource(this->fragmentShader, 1, &src, NULL);
		glCompileShader(this->fragmentShader);
		break;
	default:
		logger.logOut(LOG_LVL_ERR, "Fatal Error: Shader type unkown");
		break;
	}
}

void Renderer::compileProgram()
{
	// Create shader program
	this->shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, this->vertexShader);
	glAttachShader(shaderProgram, this->fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete unessecary shaders
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
}

void Renderer::generateBuffers(float vertices[])
{
	// Vertex Buffers
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Vertex Arrays
	glGenVertexArrays(1, &this->VAO);

	glBindVertexArray(this->VAO);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBindVertexArray(0);
}

void Renderer::generateEBO(unsigned int indices[])
{
	glGenBuffers(1, &this->EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool Renderer::validateShader(GLuint shader, ShaderType type)
{
	int success;
	char infoLog[512];

	glGetProgramiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		switch (type)
		{
		case VERTEX:
		case FRAGMENT:
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
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

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;

		return false;
	}

	return true;
}

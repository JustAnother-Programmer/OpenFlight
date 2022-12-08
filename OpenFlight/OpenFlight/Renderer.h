/*
* Copyright (c) 2022 - The OpenFlight Team
*
* Renderer.h
*/

#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Types.h"
#include "Logger.h"

class Renderer
{
public:
	bool init(Logger primaryLogger);
	void cleanup();
	void setup(float* vertices);
	void render();
	void clearScreen(float r, float g, float b, float a);
private:
	// TODO: Maybe create a struct to hold renderer data
	
	// Buffers
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	// Shaders
	GLuint vertexShader;
	GLuint fragmentShader;

	// Programs
	GLuint shaderProgram;

	// Systems
	Logger logger;

	// Functions
	void compileShader(ShaderType type, const char* src);
	void compileProgram();

	void generateBuffers(float* vertices);

	bool validateShader(GLuint shader, ShaderType type);
	bool validateProgram(GLuint program);
};
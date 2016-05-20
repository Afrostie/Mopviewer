#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
public:
	Shader();
	~Shader();
	
	std::string loadShader(const GLchar* shaderPath);
	//Compile the given shaders
	void compileShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//Use the compiled shader
	void Use();
	GLuint Program;
	GLuint returnProgram();
private:
	//Program ID;
	
	const GLchar* _vertexCode;
	const GLchar* _fragmentCode;
};


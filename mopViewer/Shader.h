#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
	//Current Program ID
	GLuint Program;
	//Read and build the shader in constructor
	std::string loadShader(const GLchar* shaderPath);
	void compileShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//Use shader
	void Use();
};


#endif
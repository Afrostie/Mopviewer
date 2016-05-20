/*
	Load Shaders from given file path
	Based on the shader tutorials from learnopengl.com, planning to rewrite my own version soon
*/


#include "Shader.h"



Shader::Shader()
{
}


Shader::~Shader()
{
}

std::string Shader::loadShader(const GLchar* shaderPath)
{
	// Load the contents of the specified file into a string
	std::ifstream inFileStream(shaderPath);
	std::string shaderCode((std::istreambuf_iterator<char>(inFileStream)),
		(std::istreambuf_iterator<char>()));
	inFileStream.close();
	return shaderCode;

}

void Shader::compileShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	GLint success;
	GLchar errorLog[512];

	auto vertexSource = Shader::loadShader(vertexPath);
	auto fragmentSource = Shader::loadShader(fragmentPath);
	
	_vertexCode = vertexSource.c_str();
	_fragmentCode = fragmentSource.c_str();

	//Compile the Vertex Shader
	auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &_vertexCode, nullptr);
	glCompileShader(vertex);

	//Output the errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, errorLog);
		std::cerr << "ERROR: Vertex Shader Did Not Compile. Error is: " << errorLog << std::endl;
	}

	//Compile the Fragment Shader
	auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &_fragmentCode, nullptr);
	glCompileShader(fragment);

	//Output the errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, errorLog);
		std::cerr << "ERROR: Fragment Shader Did Not Compile. Error is: " << errorLog << std::endl;
	}

	//Attach shaders to program
	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	

	//Link Program and Output errors
	glLinkProgram(Program);
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, nullptr, errorLog);
		std::cerr << "Program Did not Link. Error is: " << errorLog << std::endl;
	}

	//Shaders not required after Attaching and Linking
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(Program);
}

GLuint Shader::returnProgram()
{
	return Program;
}

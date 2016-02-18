/*
	Based off of basic classes given in the tutorials at learnopengl.com
*/

#include "Shader.h"
std::string Shader::loadShader(const GLchar* shaderPath) {
  // Load the contents of the specified file into a string
  std::ifstream ifs(shaderPath);
  std::string shaderCode((std::istreambuf_iterator<char>(ifs)),
                         (std::istreambuf_iterator<char>()));
  ifs.close();
  return shaderCode;
}

void Shader::compileShader(const GLchar* vertexPath,
                           const GLchar* fragmentPath) {
  // Compile Shaders
  GLuint vertex, fragment;
  GLint success;
  GLchar errorLog[512];
  //Load the shaders from file and convert them to required format
  std::string vertexCode = Shader::loadShader(vertexPath);
  std::string fragmentCode = Shader::loadShader(fragmentPath);
  const GLchar* vertexSource = vertexCode.c_str();
  const GLchar* fragmentSource = fragmentCode.c_str();
  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexSource, NULL);
  glCompileShader(vertex);
  // Output any errors
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, errorLog);
    std::cout << "> ERROR: Vertex Shader Did Not Compile" << std::endl;
  }

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentSource, NULL);
  glCompileShader(fragment);
  // Output fragment errors
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, errorLog);
    std::cout << "> ERROR: Fragment Shader Did Not Compile" << std::endl;
  }

  // Shader Program
  this->Program = glCreateProgram();
  glAttachShader(this->Program, vertex);
  glAttachShader(this->Program, fragment);
  glLinkProgram(this->Program);
  // Print any linking errors
  glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->Program, 512, NULL, errorLog);
    std::cout << "> ERROR: Shader Did Not Link" << std::endl;
  }

  // Clear up memory
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::Use() { glUseProgram(this->Program); }
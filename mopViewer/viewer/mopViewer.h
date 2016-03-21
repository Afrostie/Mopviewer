#ifndef MOPVIEWER_H
#define MOPVIEWER_H

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class mopViewer
{
public:
	GLFWwindow* currentWindow;
	void init(const GLuint width, const GLuint height);
	static void deleteBuffer(GLuint VAO, GLuint VBO, GLuint EBO);
	void render();
};


#endif
#ifndef windowManager_H
#define windowManager_H

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class windowManager
{
public:
	
	void init();
	void windowManager::getScreenSize();
	//Functions to return dimensions of window
	int getWindowHeight();
	int getWindowWidth();
	GLFWwindow* getWindow();
private:
	//The window height
	int width;
	int height;
	GLFWwindow* currentWindow;
};


#endif

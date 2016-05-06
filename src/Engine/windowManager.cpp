/**
 * A utility class
 * I plan to move more functionality to this
 */


#include "windowManager.h"

void windowManager::init()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	if (!glfwInit()) {
		std::cerr << "GLFW failed to init" << std::endl;
		exit(0);
	}
#ifdef COMPATABILITY
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif

	getScreenSize();

	// Create a GLFWwindow object that we can use for GLFW's functions
	currentWindow = glfwCreateWindow(width, height, "viewer", NULL, NULL);
	glfwMakeContextCurrent(currentWindow);
#ifdef COMPATABILITY
#else
	glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to init" << std::endl;
		exit(0);
	}

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
}
void windowManager::getScreenSize() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	width = mode->width;
	height = mode->height;
}

int windowManager::getWindowHeight() {
	return height;
}

int windowManager::getWindowWidth() {
	return width;
}

GLFWwindow* windowManager::getWindow() {
	return currentWindow;
}
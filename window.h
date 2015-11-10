// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>
class testWindow {
	public:
	testWindow(void);
	~testWindow(void);
  void init();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

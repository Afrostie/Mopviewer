// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "../mopLoader/mopfile/MopFile.h"
class testWindow {
 public:
  testWindow(void);
  ~testWindow(void);
  void init(std::string fileName, int skipCount);
  static void key_callback(GLFWwindow* window, int key, int scancode,
                           int action, int mode);
  GLfloat deltaTime;  // Time between current frame and last frame
  GLfloat lastFrame;
  MopFile* mopfile;
  MopState* mopstate;
};

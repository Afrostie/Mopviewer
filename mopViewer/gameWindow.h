#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader Class
#include "Shader.h"

//Camera Class
#include "Camera.h"

//viewer
#include "mopViewer.h"

//Texture Class
#include "Texture.h"


#include "../mopLoader/mopfile/MopFile.h"
class gameWindow {
 public:
  gameWindow(void);
  ~gameWindow(void);
  void init(std::string fileName, int skipCount);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  void doMovement();
  GLfloat deltaTime;  // Time between current frame and last frame
  GLfloat lastFrame;
  MopFile* mopfile;
  MopState* mopstate;
};

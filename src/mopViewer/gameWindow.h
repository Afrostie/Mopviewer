#include <iostream>
// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

//glm used for the matrix's
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader Class
#include "../Engine/Shader.h"

//Camera Class
#include "../Engine/Camera.h"

//viewer
#include "../Engine/windowManager.h"

//Texture Class
#include "../Engine/Texture.h"

//Model Class
#include "../Engine/model.h"

#include "../../Mopfile/MopFile.h"

//Used to load the mopfiles on a seperate thread
#include <thread>

class gameWindow {
 public:
  gameWindow(void);
  ~gameWindow(void);
  //Handles creation of the window and sets up the scene
  void init(std::string fileName, float skipCount);
  //Create the callback functions for user input
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  //This thread will run the mopfile loading
  static void threadFunc(MopState* mopstate1, MopFile* mopfile1, Model newModel);
  void doMovement();
  // Time between current frame and last frame
  GLfloat deltaTime;
  GLfloat lastFrame;

  MopFile* mopfile;
  MopState* mopstate;



};

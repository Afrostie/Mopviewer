/**
 * This is code taken from an OpenGL tutorial http://www.opengl-tutorial.org/
 * Using this to learn OpenGL.
 */

#include "gameWindow.h"

gameWindow::gameWindow(void) {}
gameWindow::~gameWindow(void) {}

GLfloat mixValue = 0.2f;
GLint WIDTH = 1600;
GLint HEIGHT = 900;
float skips = 0;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
bool keys[1024];
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f;   // Time of last frame
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
  gameWindow newWindow;

bool loadStates = true;

 void gameWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      keys[key] = true;
    else if (action == GLFW_RELEASE)
      keys[key] = false;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    std::cout << "> Current Skips Value: " << skips << std::endl;
    skips += 1;
    if(skips >= 9)
      skips = 9;
  }

  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
    std::cout << "> Current Skips Value: " << skips << std::endl;
    skips -= 1;
    if(skips <= 1)
      skips = 1;
  }


};

 void gameWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
};

 void gameWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
};

void gameWindow::doMovement()
{
  gameWindow mainGame;
  // Camera controls
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, this->deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, this->deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, this->deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, this->deltaTime);
};


void gameWindow::threadFunc(MopState* mopstate1,   MopFile* mopfile1){

  std::cout << "Started thread" << std::endl;
  while(loadStates == true){
    newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
    std::cout << "Loaded State" << std::endl;
    //newWindow.mopstate = mopstate1;
  }
};



mopViewer activeWindow;
Texture activeTexture;
  //gameWindow mainGame;


void gameWindow::init(std::string fileName, float skipCount) {
  skips = skipCount;
  activeWindow.init(WIDTH, HEIGHT);
  glfwSetCursorPosCallback(activeWindow.currentWindow, gameWindow::mouse_callback);
  // Set the required callback functions
  glfwSetKeyCallback(activeWindow.currentWindow, gameWindow::key_callback);
  glfwSetScrollCallback(activeWindow.currentWindow, gameWindow::scroll_callback);

  Shader myShader;
  myShader.compileShader("Resources/vertex.vert", "Resources/fragment.frag");

  // Set up vertex data (and buffer(s)) and attribute pointers as well as color values
  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  newWindow.mopstate = new MopState();
  newWindow.mopfile = new MopFile();
  newWindow.mopfile->setFilename(fileName);
 newWindow.mopfile->openMopfileReader();
  newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
  std::cout << "Item Count: " << newWindow.mopstate->getItemCount() << std::endl;


  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  //glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  //glEnableVertexAttribArray(1);
  // TexCoord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0); // Unbind VAO

  /*
  Texture Stuff
  */
  //Create texture ID

  GLuint texture1 = activeTexture.createTexture("Resources/container.jpg");
  /*
  2nd Texture Stuff
  */
  GLuint texture2 = activeTexture.createTexture("Resources/awesomeface.png");

  // Bind Texture



  //Uncomment for wireframe mode, useful for debugging
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  void* arg;

  std::thread threadOne(gameWindow::threadFunc, mopstate, mopfile);
  //threadOne.join();

  // Game loop
  while (!glfwWindowShouldClose(activeWindow.currentWindow))
  {
    //mopstate = mopfile->readCyclingState(skips);
    //Calculate time since last frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    std::cout << "> Time Since Last Frame: " << deltaTime << std::endl;
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    // Render
    gameWindow::doMovement();

    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(myShader.Program, "myTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(myShader.Program, "myTexture2"), 1);
    glUniform1f(glGetUniformLocation(myShader.Program, "mixValue"), mixValue);


    //Use our shaders
    myShader.Use();
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    view = camera.GetViewMatrix();
    projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000000000.0f);


    GLint modelLocation = glGetUniformLocation(myShader.Program, "model");
    GLint viewLocation = glGetUniformLocation(myShader.Program, "view");
    GLint projectionLocation = glGetUniformLocation(myShader.Program, "projection");

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));



    //Draw the objects
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    for (GLuint i = 0; i < newWindow.mopstate->getItemCount(); i++)
    {
      glm::mat4 model;
      model = glm::translate(model, glm::vec3(newWindow.mopstate->getMopItem(i).x/1000000000,newWindow.mopstate->getMopItem(i).y/1000000000,newWindow.mopstate->getMopItem(i).z/1000000000));
      GLfloat angle = 20.0f * i;
      model = glm::scale(model, glm::vec3(newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation));
      //model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(activeWindow.currentWindow);
  }
  // Properly de-allocate all resources once they've outlived their purpose
  //activeWindow.deleteBuffer(VAO, VBO, EBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  loadStates = false;
  threadOne.detach();
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
}

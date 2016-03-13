/**
 * This is code taken from an OpenGL tutorial http://www.opengl-tutorial.org/
 * Using this to learn OpenGL.
 */

#include "gameWindow.h"

gameWindow::gameWindow(void) {
}
gameWindow::~gameWindow(void) {
}

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
unsigned long int scaler = 1000000000;

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

        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
                std::cout << "> Current Skips Value: " << skips << std::endl;
                skips += 1;
                if(skips >= 10)
                        skips = 10;
        }

        if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
                std::cout << "> Current Skips Value: " << skips << std::endl;
                skips -= 1;
                if(skips <= 1)
                        skips = 1;
        }
        if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
                std::cout << "> Current Scaler Value: " << scaler << std::endl;
                scaler = scaler - 100000000;
                if(scaler <= 0){
                  scaler = 1;
                }
        }
        if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
                std::cout << "> Current Scaler Value: " << scaler << std::endl;
                scaler =  scaler + 100000000;
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
        GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left

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
        while(loadStates == true) {
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

        Shader objectShader;
        objectShader.compileShader("Resources/Shaders/object.vert", "Resources/Shaders/object.frag");

        Model newModel("Resources/Model/sphere/sphere.obj");
        Model cubeModel("Resources/Model/cube/crate.obj");

        // Set up vertex data (and buffer(s)) and attribute pointers as well as color values

        newWindow.mopstate = new MopState();
        newWindow.mopfile = new MopFile();
        newWindow.mopfile->setFilename(fileName);
        newWindow.mopfile->openMopfileReader();
        newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
        std::cout << "Item Count: " << newWindow.mopstate->getItemCount() << std::endl;



        //Uncomment for wireframe mode, useful for debugging
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
                //std::cout << "> Time Since Last Frame: " << deltaTime << std::endl;
                // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
                glfwPollEvents();
                // Render
                gameWindow::doMovement();

                // Render
                // Clear the colorbuffer
                glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                //Use our shaders
                objectShader.Use();
                glm::mat4 model;
                glm::mat4 view;
                glm::mat4 projection;


                projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 1000000000.0f);
                view = camera.GetViewMatrix();
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));




                for (GLuint i = 0; i < newWindow.mopstate->getItemCount(); i++)
                {

                        glm::mat4 model;
                        model = glm::translate(model, glm::vec3(newWindow.mopstate->getMopItem(i).x/scaler,newWindow.mopstate->getMopItem(i).y/scaler,newWindow.mopstate->getMopItem(i).z/scaler)); // Translate it down a bit so it's at the center of the scene
                        model = glm::scale(model, glm::vec3(newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation)); // It's a bit too big for our scene, so scale it down
                        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        newModel.Draw(objectShader);

                }


                model = glm::translate(model, glm::vec3(0,0,0)); // Translate it down a bit so it's at the center of the scene
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                cubeModel.Draw(objectShader);

                // Swap the screen buffers
                glfwSwapBuffers(activeWindow.currentWindow);
        }
        // Properly de-allocate all resources once they've outlived their purpose
        loadStates = false;
        threadOne.detach();
        // Terminate GLFW, clearing any resources allocated by GLFW.
        glfwTerminate();
}

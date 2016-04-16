/**
 * Some sections of code are based on Tutorials from learnopengl.com
 * This class manages everything related to the OpenGL viewer
 */

#include "gameWindow.h"

gameWindow::gameWindow(void) {
}
gameWindow::~gameWindow(void) {
}
//Height and WIDTH of screen
//TODO: Use glfw to return user's screen size
GLint WIDTH = 1366;
GLint HEIGHT = 768;
//Default skip value for particle loading
float skips = 0;
//Create the camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Used to keep track of mouse and keyboard input
bool firstMouse = true;
bool keys[1024];
//These variables keep track of performance using time between frames
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f;   // Time of last frame
//Middle of screen, to move mouse to correct position
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;


//Value that is used to scale all particles down
//TODO: proper scaling function
long long int scaler = 10000000;
//Used to keep track of how many states have been loaded
int loadedStates = 0;
//Whether to continue loading states
bool loadStates = true;


mopViewer activeWindow;
Texture activeTexture;
gameWindow newWindow;
void gameWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
        if (key >= 0 && key < 1024) {
                if (action == GLFW_PRESS)
                        keys[key] = true;
                else if (action == GLFW_RELEASE)
                        keys[key] = false;
        }
        //Close window on pressing esc
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

        //Next 4 sections handle increasing/decreasing scalar/skips value
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
                std::cout << "> Current Skips Value: " << skips << std::endl;
                skips += 0.5;
                if(skips >= 10)
                        skips = 10;
        }

        if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
                std::cout << "> Current Skips Value: " << skips << std::endl;
                skips -= 0.5;
                if(skips <= 1)
                        skips = 0.5;
        }
        if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
                std::cout << "> Current Scaler Value: " << scaler << std::endl;
                scaler /= 10;
                if(scaler <= 0){
                  scaler = 1;
                }
        }
        if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
                std::cout << "> Current Scaler Value: " << scaler << std::endl;
                scaler *= 10;
        }
        //Increase/decrease camera speed
        if(key == GLFW_KEY_MINUS && action == GLFW_PRESS){
            camera.decreaseSpeed();
        }
        if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
            camera.increaseSpeed();
        }
};

void gameWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
        //Moves mouse to center of screen if its the first time the window is opened
        //Prevents wierd movement when starting program
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
//Currently not used, but allows for the use of scroll wheel input
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

//This thread constantly loads particles in the background
//TODO: find better way to end the thread
void gameWindow::threadFunc(MopState* mopstate1,   MopFile* mopfile1){
        std::cout << "Started thread" << std::endl;
        while(loadStates) {
                newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
                std::cout << "Loaded State: " << loadedStates << std::endl;
                loadedStates++;
        }
};



void gameWindow::init(std::string fileName, float skipCount) {
        skips = skipCount;
        //Initalises some of the OpenGL contex and sets some settings
        activeWindow.init(WIDTH, HEIGHT);

        // Set the required callback functions
        glfwSetCursorPosCallback(activeWindow.currentWindow, gameWindow::mouse_callback);
        glfwSetKeyCallback(activeWindow.currentWindow, gameWindow::key_callback);
        glfwSetScrollCallback(activeWindow.currentWindow, gameWindow::scroll_callback);

        //Compile the fragment and vertex shader
        Shader objectShader;
        objectShader.compileShader("Resources/Shaders/object.vert", "Resources/Shaders/object.frag");

        //Load sphere model
        Model newModel("Resources/Model/sphere/sphere.obj");

        //Create the Mopfile and MopState and load an inital state
        newWindow.mopstate = new MopState();
        newWindow.mopfile = new MopFile();
        newWindow.mopfile->setFilename(fileName);
        newWindow.mopfile->openMopfileReader();
        newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
        std::cout << "Item Count: " << newWindow.mopstate->getItemCount() << std::endl;

        //Uncomment for wireframe mode, useful for debugging
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //Create the thread and attach the thread function
        std::thread threadOne(gameWindow::threadFunc, mopstate, mopfile);

        // Game loop
        while (!glfwWindowShouldClose(activeWindow.currentWindow))
        {
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

                //Use the shader
                objectShader.Use();

                //Using glm to create the matrix's for camera control
                glm::mat4 model;
                glm::mat4 view;
                glm::mat4 projection;

                //Setup projection matrix, high final value means you can see objects extremly far away
                projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 1000000000.0f);
                view = camera.GetViewMatrix();
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


                //Place every object in the scene
                for (GLuint i = 0; i < newWindow.mopstate->getItemCount(); i++)
                {
                        glm::mat4 model;
                        model = glm::translate(model, glm::vec3(newWindow.mopstate->getMopItem(i).x/scaler,newWindow.mopstate->getMopItem(i).y/scaler,newWindow.mopstate->getMopItem(i).z/scaler)); // Translate it down a bit so it's at the center of the scene
                        model = glm::scale(model, glm::vec3(newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation,newWindow.mopstate->getMopItem(i).visualRepresentation)); // It's a bit too big for our scene, so scale it down
                        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        newModel.Draw(objectShader);
                }

                // Swap the screen buffers
                glfwSwapBuffers(activeWindow.currentWindow);
        }
        // Properly de-allocate all resources once they've outlived their purpose
        loadStates = false;
        threadOne.detach();
        // Terminate GLFW, clearing any resources allocated by GLFW.
        glfwTerminate();
}

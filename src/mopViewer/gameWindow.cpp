/**
* Some sections of code are based on Tutorials from learnopengl.com
* This class manages everything related to the OpenGL viewer
*/

#include "gameWindow.h"

gameWindow::gameWindow(void) {

}
gameWindow::~gameWindow(void) {
}

//mopViewer activeWindow;
Texture activeTexture;
gameWindow newWindow;
windowManager activeWindow;


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
GLfloat lastX = activeWindow.getWindowWidth() / 2;
GLfloat lastY = activeWindow.getWindowHeight() / 2;


//Value that is used to scale all particles down
//TODO: proper scaling function
GLuint scaler = 1000000000;
//Used to keep track of how many states have been loaded
int loadedStates = 0;
//Whether to continue loading states
bool loadStates = true;



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
		if (skips >= 10)
			skips = 10;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		std::cout << "> Current Skips Value: " << skips << std::endl;
		skips -= 0.5;
		if (skips <= 1)
			skips = 0.5;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		std::cout << "> Current Scaler Value: " << scaler << std::endl;
		scaler /= 10;
		if (scaler <= 0) {
			scaler = 1;
		}
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		std::cout << "> Current Scaler Value: " << scaler << std::endl;
		scaler *= 10;
	}
	//Increase/decrease camera speed
	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		camera.decreaseSpeed();
	}
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
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
void gameWindow::threadFunc(MopState* mopstate1, MopFile* mopfile1, Model newModel) {
	std::cout << "Started thread" << std::endl;
	while (loadStates) {
		//newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
		//   std::cout << "Loaded State: " << loadedStates << std::endl;
		//loadedStates++;
	}
	exit(0);
};



void gameWindow::init(std::string fileName, float skipCount) {
	skips = skipCount;
	//Initalises some of the OpenGL contex and sets some settings
	activeWindow.init();

	// Set the required callback functions
	glfwSetCursorPosCallback(activeWindow.getWindow(), gameWindow::mouse_callback);
	glfwSetKeyCallback(activeWindow.getWindow(), gameWindow::key_callback);
	glfwSetScrollCallback(activeWindow.getWindow(), gameWindow::scroll_callback);

	//Compile the fragment and vertex shader
	Shader objectShader;
	Shader instanceShader;
#ifdef COMPATABILITY
	objectShader.compileShader("Resources/Shaders/object_comp.vert", "Resources/Shaders/object_comp.frag");
#else
	//objectShader.compileShader("Resources/Shaders/object.vert", "Resources/Shaders/object.frag");
	instanceShader.compileShader("Resources/Shaders/instance.vert", "Resources/Shaders/instance.frag");
#endif
	//Load sphere model
	Model newModel("Resources/Model/sphere/sphere.obj");


	//Create the Mopfile and MopState and load an inital state
	newWindow.mopstate = new MopState();
	newWindow.mopfile = new MopFile();
	newWindow.mopfile->setFilenameViewer(fileName);
	newWindow.mopfile->openMopfileReader();
	newWindow.mopstate = newWindow.mopfile->readCyclingState(skips);
	std::cout << "Item Count: " << newWindow.mopstate->getItemCount() << std::endl;





	GLuint amount =  newWindow.mopstate->getItemCount();
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];

	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]
		model = glm::translate(model, glm::vec3(newWindow.mopstate->getMopItem(i).x / scaler, newWindow.mopstate->getMopItem(i).y / scaler, newWindow.mopstate->getMopItem(i).z / scaler));
		model = glm::scale(model, glm::vec3(newWindow.mopstate->getMopItem(i).visualRepresentation, newWindow.mopstate->getMopItem(i).visualRepresentation, newWindow.mopstate->getMopItem(i).visualRepresentation));

		modelMatrices[i] = model;

	}
	//This code was given in the Instanced tutorial at learnopengl.com
	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	// NOTE: We're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// Normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	for (GLuint i = 0; i < newModel.meshes.size(); i++)
	{
		GLuint VAO = newModel.meshes[i].VAO;
		GLuint buffer;
		glBindVertexArray(VAO);
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		// Set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	//Uncomment for wireframe mode, useful for debugging
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Create the thread and attach the thread function
	//std::thread threadOne(gameWindow::threadFunc, mopstate, mopfile, newModel);

	// Game loop
	while (!glfwWindowShouldClose(activeWindow.getWindow()))
	{
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
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		

		//Using glm to create the matrix's for camera control
		glm::mat4 view;
		glm::mat4 projection;

		//Setup projection matrix, high final value means you can see objects extremly far away
		projection = glm::perspective(camera.Zoom, (float)activeWindow.getWindowWidth() / (float)activeWindow.getWindowHeight(), 0.1f, 1000000000.0f);
		view = camera.GetViewMatrix();
		instanceShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	

		// Draw meteorites
		instanceShader.Use();
		glBindTexture(GL_TEXTURE_2D, newModel.textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
		for (GLuint i = 0; i < newModel.meshes.size(); i++)
		{
			glBindVertexArray(newModel.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, newModel.meshes[i].vertices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		// Swap the screen buffers
		glfwSwapBuffers(activeWindow.getWindow());
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loadStates = false;
	//threadOne.detach();
	delete[] modelMatrices;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}
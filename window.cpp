/**
 * This is code taken from an OpenGL tutorial http://www.opengl-tutorial.org/
 * Using this to learn OpenGL.
 */

#include "window.h"

testWindow::testWindow(void){

}
testWindow::~testWindow(void){

}

// Code for vertex shader, should be moved to external file
const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                   "}\0";
// Code for fragment shader, should be moved to external file
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";
void testWindow::init ( void )
{


	glfwInit();                                                         //Init glfw
	//For purposes of learning get OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Using core profile of OpenGL prevents use of any legacy functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                                                         //Stop the window from being resized
	//Create a glfw window, size 800 x 600
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {                                                         //Some error checking
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return -1;
	}

	glfwMakeContextCurrent(window);                                                         //Make this window the main window
	//Setup the keypress callback
	glfwSetKeyCallback(window, &testWindow::key_callback);

	//Makes sure glew uses te latest techniques
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {                                                         //Check glew is set up ok
		std::cout << "Failed to initalise GLEW" << std::endl;
	}


	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f // Top Left
	};
	GLuint indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};
int skip = 22;
  mopfile = new MopFile();
  mopfile->setFilename("Testing_Project.mop");
  mopfile->openMopfileReader();
  mopstate = new MopState();
  mopstate = mopfile->readCyclingState(skip);
  std::cout << "Item Count: " << mopstate->getItemCount() << std::endl;



	//Create a vertex buffer object, as it is storing co-ordinates needs to be a GL_ARRAY_BUFFER
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Put the data from vertices into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	 *	First parameter is which attribute to configure, n our shader we used location 0, so 0
	 *	Next value is the size of each vertex, in this case we are using 3D so 3 points
	 *	The data is of type GLfloat
	 *	GL_FALSE tells OpenGL not to normalise data as it is already normalised
	 *	Next of the distance between each vertex in the array, for each vertex we have 3 glfloats
	 *	Data is at start of array, so position 8
	 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//Enable all the above attributes
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create our shader object
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Pass in the shader and compile it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Create a couple of variables to store any error information
	GLint success;
	GLchar infoLog[512];
	//In compilation of shader fails, will output error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//Same as above, except for fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
//In compilation of shader fails, will output error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create a Program for our shaders. This will combine the current shaders for output to screen
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	//Here I attached each shader to the program, then link them together
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Similar to above, checking for any errors with linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Linking Failed!\n" << infoLog << std::endl;
	}

	//Can delete the shaders once they have been linked, no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	//Tell Opengl the window size
	glViewport(0, 0, 800, 600);

  //Displays in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Start the main game loop,

while(!glfwWindowShouldClose(window)) {
mopstate = mopfile->readCyclingState(skip);
  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  std::cout <<"Time Since Last Frame is: " << deltaTime << "s" << std::endl;
		//check for events during the loop
		glfwPollEvents();
		//Render what you want, set color and then clear for the next loop
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Every rendering call after this will use the program, in this case shaderProgram
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Swap buffers, prevent flickers or artifacts
		glfwSwapBuffers(window);
	}

//Close the window
	glfwTerminate();

}

void testWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	// Close program when esc is pressed
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

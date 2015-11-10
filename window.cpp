/**
 * This is code taken from an OpenGL tutorialhttp://www.opengl-tutorial.org/
 * Using this to learn OpenGL as well as test opening Opengl windows from nCurses.
 */

#include "window.h"
testWindow::testWindow(void){

}
testWindow::~testWindow(void){

}

void testWindow::init ( void )
{
	glfwInit(); //Init glfw
		//For purposes of learning get OpenGL version to 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//Using core profile of OpenGL prevents use of any legacy functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Stop the window from being resized
			//Create a glfw window, size 800 x 600
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr){	//Some error checking
    	std::cout << "Failed to create GLFW window" << std::endl;
  		glfwTerminate();
  		//return -1;
		}

	glfwMakeContextCurrent(window); //Make this window the main window
		//Setup the keypress callback
		glfwSetKeyCallback(window, &testWindow::key_callback);

	//Makes sure glew uses te latest techniques
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){ //Check glew is set up ok
		std::cout << "Failed to initalise GLEW" << std::endl;
	}
		//Tell Opengl the window size
	glViewport(0, 0, 800, 600);
		//Start the main game loop,
	while(!glfwWindowShouldClose(window)){
		//check for events during the loop
		glfwPollEvents();
		//Render what you want, set color and then clear for the next loop
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Swap buffers, prevent flickers or artifacts
		glfwSwapBuffers(window);
	}


	glfwTerminate(); //Close the window

}

void testWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Close program when esc is pressed
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}

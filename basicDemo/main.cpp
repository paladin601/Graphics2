#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\matrix_cross_product.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"


unsigned int windowWidth = 800;
unsigned int windowHeight = 800;
// Window title
const char* windowTitle = "Tarea CCG 3";
float aspectRatio = float(windowWidth / windowHeight);
// Window pointer
GLFWwindow* window;
// Shader object
Shader* shader;

//MVP Matrix
glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;

//Camera
Camera* camera = new Camera(glm::vec3(0,0,0));

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
bool mouseOn = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

const char* path = ".\assets\models\bonsai_256x256x256_uint8.raw";

unsigned int VAO = 0;
unsigned int VBO;

/* *
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	// Sets the OpenGL viewport size and position
	glViewport(0, 0, windowWidth, windowHeight);

}

bool isKeyPress(int key) {
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (mouseOn) {

		if (firstMouse)
		{
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = x - lastX;
		float yoffset = lastY - y;


		camera->ProcessMouseMovement(xoffset, yoffset);
		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
		lastX = windowWidth / 2.0;
		lastY = windowHeight / 2.0;
	}
	else {
		lastX = windowWidth / 2.0;
		lastY = windowHeight / 2.0;
	}
}

/**
 * Calback key press
 * */
void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (isKeyPress(GLFW_KEY_Q)) {
		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
		mouseOn = !mouseOn;
		if (mouseOn) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}


void initUserInterfaceValues()
{



}
/**
 * initialize the user interface
 * @returns{bool} true if everything goes ok
* */
bool initUserInterface()
{
	return true;
}
/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
	// Initialize glfw
	glfwInit();
	// Sets the Opegl context to Opengl 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Creates the window
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	// The window couldn't be created
	if (!window)
	{
		std::cout << "Failed to create the glfw window" << std::endl;
		glfwTerminate(); // Stops the glfw program
		return false;
	}

	// Creates the glfwContext, this has to be made before calling initGlad()
	glfwMakeContextCurrent(window);

	// Window resize callback
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetCharCallback(window, charInput);

	return true;
}

/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
	// Initialize glad
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// If something went wrong during the glad initialization
	if (!status)
	{
		std::cout << status << std::endl;
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}
/**
 * Initialize the opengl context
 * */
void initGL()
{
	// Enables the z-buffer test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sets the ViewPort
	glViewport(0, 0, windowWidth, windowHeight);
	// Sets the clear color
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

/*
** Init MVP Matrix
*/


/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */


void buildGeometry()
{
	float vertices[] = {
		// positions        // texture Coords
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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
	// Initialize the window, the interface and the glad components
	if (!initWindow() || !initGlad() )
		return false;

	// Initialize the opengl context
	initGL();

	// Loads the shader
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");


	// Loads all the geometry into the GPU
	buildGeometry();
	
	//Init values of tweakbar
	initUserInterfaceValues();

	return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow* window)
{
	// Checks if the escape key is pressed
	if (isKeyPress(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
	if (isKeyPress(GLFW_KEY_W))
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (isKeyPress(GLFW_KEY_S))
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (isKeyPress(GLFW_KEY_A))
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (isKeyPress(GLFW_KEY_D))
		camera->ProcessKeyboard(RIGHT, deltaTime);

	// Checks if the r key is pressed
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		// Reloads the shader
		delete shader;

		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

	}
}

void updateUserInterface()
{
	//Model
}



/**
 * Render Function
 * */
void render()
{
	// Clears the color and depth buffers from the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	Projection = glm::perspective(glm::radians(camera->Zoom), aspectRatio, 0.1f, 100.0f);
	View = camera->GetViewMatrix();

	shader->use();
	shader->setMat4("Model", Model);
	shader->setMat4("View", View);
	shader->setMat4("Projection", Projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


}
/*
* change speed
*/

/**
 * App main loop
 * */
void update()
{
	// Loop until something tells the window, that it has to be closed
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Checks for keyboard inputs
		processKeyboardInput(window);

		// Renders everything
		render();

		//Update
		//updateUserInterface();

		// Swap the buffer
		glfwSwapBuffers(window);

		// Check and call events
		glfwPollEvents();
	}
}


/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const* argv[])
{
	// Initialize all the app components
	if (!init())
	{
		// Something went wrong
		std::cin.ignore();
		return -1;
	}

	std::cout << "=====================================================" << std::endl
		<< "        Press Escape to close the program            " << std::endl
		<< "=====================================================" << std::endl;

	// Starts the app main loop
	update();


	//clear memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
	delete camera;
	glfwTerminate();

	return 0;
}

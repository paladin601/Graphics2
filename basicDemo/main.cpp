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


#include "TextureLoader.h"
#include "Shader.h"
#include "Camera.h"

using namespace std;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

float aspectRatio = float(windowWidth / windowHeight);
const char *windowTitle = "CCG Tarea";

unsigned int VBO;
unsigned int VAO;
unsigned int textureID;
int max;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
bool mouseOn = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


GLFWwindow *window;
TextureLoader loaderTexture;
Shader *shader;


Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::mat4 MVP, Projection, View, Model;


/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */


bool isKeyPress(int key) {
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
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

void processKeyboardInput(GLFWwindow *window)
{
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
	if (isKeyPress(GLFW_KEY_R))
	{
		// Reloads the shader
		delete shader;

		shader = new Shader("assets/shaders/pointLightLamp.vert", "assets/shaders/pointLightLamp.frag");

	}

}

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{


}


void resize(GLFWwindow *window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	// Sets the OpenGL viewport size and position
	glViewport(0, 0, windowWidth, windowHeight);
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
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, keyInput);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetCharCallback(window, charInput);
	return true;
}

/**z
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


	// Sets the ViewPort
	glViewport(0, 0, windowWidth, windowHeight);
	// Sets the clear color
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void buildGeometry()
{

	float vertices[] = {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

}


bool init()
{
	// Initialize the window, and the glad components
	if (!initWindow() || !initGlad())
		return false;

	// Initialize the opengl context
	initGL();
	buildGeometry();
	

	// Loads the shader
	shader = new Shader("assets/shaders/pointLightLamp.vert", "assets/shaders/pointLightLamp.frag");
	

	return true;
}


void render()
{
	// Clears the color and depth buffers from the frame buffer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/** Draws code goes here **/
	// Use the shader

	// craer la matriz de vista
	Projection = glm::perspective(glm::radians(camera->Zoom), aspectRatio, 0.1f, 100.0f);
	View = camera->GetViewMatrix();

	/*paint volumen*/
	shader->use();
	shader->setMat4("projectionMatrix", Projection);
	shader->setMat4("viewMatrix", View);


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



	glBindVertexArray(VAO);
	// Renders the triangle gemotry
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader->setMat4("modelMatrix", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);


}


/**
 * App main loop
 * */

void update()
{
	// Loop until something tells the window, that it has to be closed
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Checks for keyboard inputs
		processKeyboardInput(window);

		render();
		// Renders everything

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

int main(int argc, char const *argv[])
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
	// Deletes the texture from the gpu
	glDeleteTextures(1, &textureID);

	// Deletes the vertex array from the GPU

	glDeleteVertexArrays(1, &VAO);
	
	glDeleteBuffers(1, &VBO);


	// Stops the glfw program
	glfwTerminate();
	delete shader;
	delete camera;




	return 0;
}
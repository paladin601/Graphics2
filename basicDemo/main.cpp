#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\matrix_cross_product.hpp>


#include "ObjLoader.h"
#include "TextureLoader.h"
#include "Shader.h"
#include "Camera.h"
#include "UserInterface.h"
#include "Texture.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "Geometry.h"

using namespace std;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

float aspectRatio = float(windowWidth / windowHeight);
const char *windowTitle = "CCG Tarea";

unsigned int VBO[5];
unsigned int VAO;
unsigned int textureID;

int n, cont, i, max, materialType;
int auxPicked = 0;
int meshPicked = 3;
int geometryPicked = 0;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
bool mouseOn = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;



GLFWwindow *window;
ObjLoader loaderGeometry;
TextureLoader loaderTexture;
vector<Mesh *> meshes;
CUserInterface * userInterface;
vector<Shader *> shaders;
Shader *shader;
Texture *textureIDS = new Texture();
DirectionalLight *Ambient;
SpotLight *spotLight;
vector<PointLight *> pointLights;
Geometry *geo;
Mesh *mesh;
vector<unsigned int>VBOaux;
Camera *camera = new Camera(glm::vec3(-8.0f, 4.0f, 5.0f));
glm::mat4 MVP, Projection, View, Model, ModelView;




/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */

bool meshPickedChange(int id) {
	return id == meshPicked;
}

bool initUserInterface()
{
	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;

	userInterface = CUserInterface::Instance();
	userInterface->reshape();

	return true;
}

bool isKeyPress(int key) {
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (TwEventMouseButtonGLFW(button, action))
		return;
}



// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
	if (mouseOn) {

		if (firstMouse)
		{
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = x - lastX;
		float yoffset = lastY - y;

		lastX = x;
		lastY = y;

		camera->ProcessMouseMovement(xoffset, yoffset);
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
	}
	if (isKeyPress(GLFW_KEY_Z)) {
		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
		lastX = windowWidth / 2.0;
		lastY = windowHeight / 2.0;
	}
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
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
		shaders.clear();

		shader = new Shader("assets/shaders/pointLightLamp.vert", "assets/shaders/pointLightLamp.frag");
		shaders.push_back(shader);
		shader = new Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
		shaders.push_back(shader);
		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		shaders.push_back(shader);
		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/orenNayar.frag");
		shaders.push_back(shader);
		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/cookTorrance.frag");
		shaders.push_back(shader);

	}

}

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (TwEventKeyGLFW(key, action))
		return;
}


void resize(GLFWwindow *window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	userInterface->reshape();
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
	glEnable(GL_BLEND);

	// Sets the ViewPort
	glViewport(0, 0, windowWidth, windowHeight);
	// Sets the clear color
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */

void buildGeometry()
{
	cont = mesh->getGeometryLength();
	for (n = 0; n < cont; n++) {
		geo = mesh->getGeometry(n);
		vector<glm::vec3> vertex;
		vector<glm::vec3> normal;
		vector<glm::vec3> tangent;
		vector<glm::vec3> bitangent;
		vector<glm::vec2> uv;
		vertex = geo->getFaces();
		uv = geo->getUVs();
		normal = geo->getNormals();
		tangent = geo->getTangent();
		bitangent = geo->getBitangent();
		// Creates on GPU the vertex array
		glGenVertexArrays(1, &VAO);
		// Creates on GPU the vertex buffer object
		glGenBuffers(5, VBO);
		geo->setVAO(VAO);
		geo->setVBO(VBO[0]);
		geo->setVBO(VBO[1]);
		geo->setVBO(VBO[2]);
		geo->setVBO(VBO[3]);
		geo->setVBO(VBO[4]);
		// Binds the vertex array to set all the its properties
		glBindVertexArray(VAO);
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
		// Sets the vertex attributes
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		// buffer de textura 
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(glm::vec3), &tangent[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, bitangent.size() * sizeof(glm::vec3), &bitangent[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindVertexArray(0);
	}
}

/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */




 //textureIDS->setTexture(cubeID);
 //textureIDS->setTexture(id);


/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */

void updateDataMesh() {

	meshes[meshPicked]->setMaterialType(userInterface->getMeshMaterialPicked());
	meshes[meshPicked]->setRotate(userInterface->getMeshRotate());
	meshes[meshPicked]->setTranslate(userInterface->getMeshTranslate());
	meshes[meshPicked]->setScale(userInterface->getMeshScale());
	meshes[meshPicked]->setIORin(userInterface->getIORin());
	meshes[meshPicked]->setIORout(userInterface->getIORout());
	meshes[meshPicked]->setKS(userInterface->getMeshKS());
	meshes[meshPicked]->setKD(userInterface->getMeshKD());
	meshes[meshPicked]->setKN(userInterface->getMeshKN());
	meshes[meshPicked]->setReflect(userInterface->getReflect());
	meshes[meshPicked]->setRefract(userInterface->getRefract());
	meshes[meshPicked]->setIReflect(userInterface->getIReflect());
	meshes[meshPicked]->setFactorReflect(userInterface->getFactorReflect());
	meshes[meshPicked]->setShininess(userInterface->getShininessGeometry());
	meshes[meshPicked]->setRoughness(userInterface->getRoughnessGeometry());
	meshes[meshPicked]->setKDepth(userInterface->getMeshKDepth());
	meshes[meshPicked]->setHeightScale(userInterface->getHeightScale());

	geo = meshes[meshPicked]->getGeometry(geometryPicked);
	geo->setTranslate(userInterface->getGeometryTranslate());
	geo->setRotate(userInterface->getGeometryRotate());
	geo->setScale(userInterface->getGeometryScale());
	geo->material.ambient = userInterface->getColorAmbientGeometry();
	geo->material.specular = userInterface->getColorSpecularGeometry();
	geo->material.diffuse = userInterface->getColorDiffuseGeometry();
}



void updateGeometrydata() {
	geo = meshes[meshPicked]->getGeometry(geometryPicked);
	userInterface->setGeometryName(geo->getName());
	userInterface->setGeometryTranslate(geo->getTranslate());
	userInterface->setGeometryScale(geo->getScale());
	userInterface->setGeometryRotate(geo->getRotate());
	userInterface->setColorAmbientGeometry(geo->material.ambient);
	userInterface->setColorDiffuseGeometry(geo->material.diffuse);
	userInterface->setColorSpecularGeometry(geo->material.specular);
}


void updateDataInterface() {
	userInterface->setGeometryPicked(0);
	userInterface->setGeometryLength(meshes[meshPicked]->getGeometryLength());
	userInterface->setMeshMaterialPicked(meshes[meshPicked]->getMaterialType());
	userInterface->setMeshTranslate(meshes[meshPicked]->getTranslate());
	userInterface->setMeshScale(meshes[meshPicked]->getScale());
	userInterface->setMeshRotate(meshes[meshPicked]->getRotate());
	userInterface->setIORin(meshes[meshPicked]->getIORin());
	userInterface->setIORout(meshes[meshPicked]->getIORout());
	
	userInterface->setMeshKD(meshes[meshPicked]->getKD());
	userInterface->setMeshKS(meshes[meshPicked]->getKS());
	userInterface->setMeshKN(meshes[meshPicked]->getKN());
	userInterface->setReflect(meshes[meshPicked]->getReflect());
	userInterface->setRefract(meshes[meshPicked]->getRefract());
	userInterface->setIReflect(meshes[meshPicked]->getIReflect());
	userInterface->setFactorReflect(meshes[meshPicked]->getFactorReflect());
	userInterface->setShininessGeometry(meshes[meshPicked]->getShininess());
	userInterface->setRoughnessGeometry(meshes[meshPicked]->getRoughness());
	userInterface->setMeshKDepth(meshes[meshPicked]->getKDepth());
	userInterface->setHeightScale(meshes[meshPicked]->getHeightScale());

	updateGeometrydata();
}

void updateUserInterface()
{
	geo = NULL;
	auxPicked = userInterface->getMeshPicked();
	if (auxPicked != meshPicked) {
		updateDataMesh();
		geo = NULL;

		meshPicked = auxPicked;
		geometryPicked = 0;
		updateDataInterface();

	}
	else {
		auxPicked = userInterface->getGeometryPicked();
		if (auxPicked != geometryPicked) {
			geometryPicked = auxPicked;
			updateGeometrydata();
		}
		else {
			updateDataMesh();
		}

	}
	Ambient->direction = userInterface->getDLLightAmbient();
	Ambient->ambient = userInterface->getColorAmbientDL();
	Ambient->diffuse = userInterface->getColorDiffuseDL();
	Ambient->specular = userInterface->getColorSpecularDL();
	Ambient->active = userInterface->getDLActive();

	spotLight->active = userInterface->getSLActive();
	spotLight->ambient = userInterface->getColorAmbientSL();
	spotLight->diffuse = userInterface->getColorDiffuseSL();
	spotLight->specular = userInterface->getColorSpecularSL();
	spotLight->cutOff = userInterface->getCutOffSL();
	spotLight->outerCutOff = userInterface->getOuterCutOffSL();
	spotLight->constant = userInterface->getConstantSL();
	spotLight->linear = userInterface->getLinearSL();
	spotLight->quadratic = userInterface->getQuadraticSL();

	pointLights[0]->active = userInterface->getPL1Active();
	pointLights[0]->ambient = userInterface->getColorAmbientPL1();
	pointLights[0]->diffuse = userInterface->getColorDiffusePL1();
	pointLights[0]->specular = userInterface->getColorSpecularPL1();
	pointLights[0]->position = userInterface->getPositionPL1();
	pointLights[0]->constant = userInterface->getConstantPL1();
	pointLights[0]->linear = userInterface->getLinearPL1();
	pointLights[0]->quadratic = userInterface->getQuadraticPL1();


	meshes[0]->getGeometry(0)->material.ambient = userInterface->getColorAmbientPL1();
	meshes[0]->getGeometry(0)->material.diffuse = userInterface->getColorDiffusePL1();
	meshes[0]->getGeometry(0)->material.specular = userInterface->getColorSpecularPL1();
	meshes[0]->setTranslate(userInterface->getPositionPL1());



	pointLights[1]->active = userInterface->getPL2Active();
	pointLights[1]->ambient = userInterface->getColorAmbientPL2();
	pointLights[1]->diffuse = userInterface->getColorDiffusePL2();
	pointLights[1]->specular = userInterface->getColorSpecularPL2();
	pointLights[1]->position = userInterface->getPositionPL2();
	pointLights[1]->constant = userInterface->getConstantPL2();
	pointLights[1]->linear = userInterface->getLinearPL2();
	pointLights[1]->quadratic = userInterface->getQuadraticPL2();

	meshes[1]->getGeometry(0)->material.ambient = userInterface->getColorAmbientPL2();
	meshes[1]->getGeometry(0)->material.diffuse = userInterface->getColorDiffusePL2();
	meshes[1]->getGeometry(0)->material.specular = userInterface->getColorSpecularPL2();
	meshes[1]->setTranslate(userInterface->getPositionPL2());

}

bool init()
{
	// Initialize the window, and the glad components
	if (!initWindow() || !initGlad() || !initUserInterface())
		return false;

	// Initialize the opengl context
	initGL();
	Ambient = new DirectionalLight();
	spotLight = new SpotLight();
	PointLight *aux1 = new PointLight();
	pointLights.push_back(aux1);
	aux1 = new PointLight();
	pointLights.push_back(aux1);

	// Loads the shader
	shader = new Shader("assets/shaders/pointLightLamp.vert", "assets/shaders/pointLightLamp.frag");
	shaders.push_back(shader);
	shader = new Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	shaders.push_back(shader);
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	shaders.push_back(shader);
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/orenNayar.frag");
	shaders.push_back(shader);
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/cookTorrance.frag");
	shaders.push_back(shader);


	vector<string> faces
	{
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/front.jpg",
		"assets/textures/skybox/back.jpg",
	};

	textureIDS->setTexture(loaderTexture.loadCubeMap(faces));
	textureIDS->setTexture(loaderTexture.load("assets/textures/white.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/normalD.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/box.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/boxSP.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/brickwall.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/arm.jpg"));//6
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/arm.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/helmet.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/helmet.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/body.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/body.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/leg.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/leg.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/glass.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/glass.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dif/hand.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/reflect/hand.jpg"));

	//18
	textureIDS->setTexture(loaderTexture.load("assets/textures/pikachu/body.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/pikachu/eye.png"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/charmander/body.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/charmander/eye.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/charmander/fire.png"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/bulbasaur/eye.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/bulbasaur/body.png"));

	//25
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/arm.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/helmet.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/body.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/leg.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/glass.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/personaje/dnn/hand.png"));
	//31
	textureIDS->setTexture(loaderTexture.load("assets/textures/brickwall_normal.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/normal_cube.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/bricks2_disp.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/bricks2.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/bricks2_normal.jpg"));

	//36
	textureIDS->setTexture(loaderTexture.load("assets/textures/wood.png"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/pruebanm.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/pruebadp.jpg"));

	textureIDS->setTexture(loaderTexture.load("assets/textures/242-diffuse.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/242-normal.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/242-reflectiveocclusion.jpg"));
	textureIDS->setTexture(loaderTexture.load("assets/textures/242-bump.jpg"));



	meshes.push_back(loaderGeometry.load("assets/obj/pointlight.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/pointlight.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/skybox.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/floor.obj"));

	meshes.push_back(loaderGeometry.load("assets/obj/cubito.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/cubito.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/personaje.obj"));

	meshes.push_back(loaderGeometry.load("assets/obj/pikachu.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/charmander.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/bulbasaur.obj"));

	meshes.push_back(loaderGeometry.load("assets/obj/cubito.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/personaje.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/personaje.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshes.push_back(loaderGeometry.load("assets/obj/esfera.obj"));

	// Loads all the geometry into the GPU
	/*
		models[0]->setMaterialType(-1);
		meshes[1]->setMaterialType(-1);
	*/

	meshes[2]->setMaterialType(0);
	meshes[2]->setTextureCubeMap(0);


	meshes[3]->setMaterialType(1);
	meshes[3]->setTextureGeometry(0, 5, 5,31,33);
	meshes[3]->setTextureCubeMap(0);



	meshes[4]->setMaterialType(1);
	meshes[4]->setTranslate(glm::vec3(-3.0f, 0.61f, 0.0f));
	meshes[4]->setTextureGeometry(0, 3, 4,32,33);
	meshes[4]->setTextureCubeMap(0);



	meshes[5]->setMaterialType(2);
	meshes[5]->setTranslate(glm::vec3(-4.5f, 0.61f, 0.0f));
	meshes[5]->setTextureGeometry(0, 3, 4,32,33);
	meshes[5]->setTextureCubeMap(0);


	meshes[6]->setTranslate(glm::vec3(-6.0f, 1.35f, 0.0f));
	meshes[6]->setMaterialType(3);
	meshes[6]->setTextureGeometry(0, 6, 7,25,33);
	meshes[6]->setTextureGeometry(2, 8, 9,26,33);
	meshes[6]->setTextureGeometry(3, 10, 11,27,33);
	meshes[6]->setTextureGeometry(4, 12, 13,28,33);
	meshes[6]->setTextureGeometry(5, 14, 15,29,33);
	meshes[6]->setTextureGeometry(6, 16, 17,30,33);
	meshes[6]->setTextureCubeMap(0);



	meshes[7]->setTranslate(glm::vec3(3.0f, 0.41f, -3.0f));
	meshes[7]->setMaterialType(1);
	meshes[7]->setKN(0);
	meshes[7]->setTextureGeometry(0, 18, 18,32,33);
	meshes[7]->setTextureGeometry(1, 19, 19,32,33);
	meshes[7]->setTextureCubeMap(0);


	meshes[8]->setTranslate(glm::vec3(4.0f, 0.55f, -3.0f));
	meshes[8]->setMaterialType(2);
	meshes[8]->setKN(0);
	meshes[8]->setTextureGeometry(0, 20, 20,32,33);
	meshes[8]->setTextureGeometry(1, 21, 21,32,33);
	meshes[8]->setTextureGeometry(2, 22, 22,32,33);
	meshes[8]->setTextureGeometry(3, 22, 22,32,33);
	meshes[8]->setTextureCubeMap(0);

	meshes[9]->setTranslate(glm::vec3(5.0f, 0.27f, -3.0f));
	meshes[9]->setMaterialType(3);
	meshes[9]->setKN(0);
	meshes[9]->setTextureGeometry(0, 24, 24,32,33);
	meshes[9]->setTextureGeometry(1, 23, 23,32,33);
	meshes[9]->setTextureGeometry(2, 24, 24,32,33);
	meshes[9]->setTextureCubeMap(0);

	meshes[10]->setTranslate(glm::vec3(-1.5f, 0.61f, -6.0f));
	meshes[10]->setTextureGeometry(0, 3, 4,32,33);
	meshes[10]->setMaterialType(3);
	meshes[10]->setTextureCubeMap(0);


	meshes[11]->setTranslate(glm::vec3(0.0f, 1.35f, -6.0f));
	meshes[11]->setMaterialType(3);
	meshes[11]->setTextureGeometry(0, 6, 7,25,33);
	meshes[11]->setTextureGeometry(2, 8, 9,26,33);
	meshes[11]->setTextureGeometry(3, 10, 11,27,33);
	meshes[11]->setTextureGeometry(4, 12, 13,28,33);
	meshes[11]->setTextureGeometry(5, 14, 15,29,33);
	meshes[11]->setTextureGeometry(6, 16, 17,30,33);
	meshes[11]->setTextureCubeMap(0);


	meshes[12]->setTranslate(glm::vec3(1.5f, 1.35f, -6.0f));
	meshes[12]->setMaterialType(3);
	meshes[12]->setTextureGeometry(0, 6, 7,25,33);
	meshes[12]->setTextureGeometry(2, 8, 9,26,33);
	meshes[12]->setTextureGeometry(3, 10, 11,27,33);
	meshes[12]->setTextureGeometry(4, 12, 13,28,33);
	meshes[12]->setTextureGeometry(5, 14, 15,29,33);
	meshes[12]->setTextureGeometry(6, 16, 17,30,33);
	meshes[12]->setTextureCubeMap(0);

	meshes[13]->setTranslate(glm::vec3(6.0f, 1.1f, -3.0f));
	meshes[13]->setMaterialType(1);
	meshes[13]->setTextureGeometry(0, 34, 34, 35, 33);
	meshes[13]->setTextureCubeMap(0);
	meshes[13]->setKDepth(1);

	meshes[14]->setTranslate(glm::vec3(6.0f, 1.1f, -5.0f));
	meshes[14]->setMaterialType(1);
	meshes[14]->setTextureGeometry(0, 34, 34, 35, 33);
	meshes[14]->setTextureCubeMap(0);
	meshes[14]->setKDepth(1);

	meshes[15]->setTranslate(glm::vec3(6.0f, 1.1f, -1.0f));
	meshes[15]->setMaterialType(1);
	meshes[15]->setTextureGeometry(0, 34, 34, 35, 33);
	meshes[15]->setTextureCubeMap(0);
	meshes[15]->setKDepth(1);

	meshes[16]->setTranslate(glm::vec3(-4.0f, 1.1f, -9.0f));
	meshes[16]->setRotate(glm::vec3(0.0f, 90.0f, 0.0f));
	meshes[16]->setMaterialType(1);
	meshes[16]->setTextureGeometry(0, 36, 36, 37, 38);
	meshes[16]->setTextureCubeMap(0);
	meshes[16]->setKDepth(1);

	meshes[17]->setTranslate(glm::vec3(0.0f, 1.1f, -9.0f));
	meshes[17]->setRotate(glm::vec3(0.0f, 90.0f, 0.0f));
	meshes[17]->setMaterialType(1);
	meshes[17]->setTextureGeometry(0, 36, 36, 37, 38);
	meshes[17]->setTextureCubeMap(0);
	meshes[17]->setKDepth(1);

	meshes[18]->setTranslate(glm::vec3(4.0f, 1.1f, -9.0f));
	meshes[18]->setRotate(glm::vec3(0.0f, 90.0f, 0.0f));
	meshes[18]->setMaterialType(1);
	meshes[18]->setTextureGeometry(0, 39, 41, 40, 42);
	meshes[18]->setTextureCubeMap(0);
	meshes[18]->setKDepth(1);

	meshes[19]->setTranslate(glm::vec3(0.0f, 2.1f, 0.0f));
	meshes[19]->setMaterialType(1);
	meshes[19]->setTextureGeometry(0, 34, 34, 35, 33);
	meshes[19]->setTextureCubeMap(0);
	meshes[19]->setKDepth(1);

	max = meshes.size();
	for (i = 0; i < max; i++) {
		mesh = meshes[i];
		buildGeometry();
	}
	// Loads the texture into the GPU

	updateDataInterface();

	return true;
}


void activeShader(int shaderSelect) {
	int kn,kdepth;
	kn = mesh->getKN();
	kdepth = mesh->getKDepth();

	MVP = Projection * View* Model;

	shaders[shaderSelect]->use();
	shaders[shaderSelect]->setVec3("PositionPL[0]", pointLights[0]->position);
	shaders[shaderSelect]->setVec3("PositionPL[1]", pointLights[1]->position);
	shaders[shaderSelect]->setVec3("PositionSP", camera->Position);
	shaders[shaderSelect]->setVec3("DirectionSP", camera->Front);
	shaders[shaderSelect]->setVec3("DirectionAL", Ambient->direction);
	shaders[shaderSelect]->setVec3("viewPos", camera->Position);
	shaders[shaderSelect]->setInt("knActive", (kn==1||kdepth==1)?1:0);



	shaders[shaderSelect]->setInt("ambientLight.Active", Ambient->active);
	shaders[shaderSelect]->setVec3("ambientLight.AmbientColor", Ambient->ambient);
	shaders[shaderSelect]->setVec3("ambientLight.DifusseColor", Ambient->diffuse);
	shaders[shaderSelect]->setInt("skybox", 0);

	shaders[shaderSelect]->setVec3("objMaterial.AmbientColor", geo->material.ambient);
	shaders[shaderSelect]->setVec3("objMaterial.DifusseColor", geo->material.diffuse);
	shaders[shaderSelect]->setFloat("objMaterial.roughness", mesh->getRoughness());
	shaders[shaderSelect]->setFloat("objMaterial.IReflect", mesh->getIReflect());
	shaders[shaderSelect]->setFloat("factReflec", mesh->getFactorReflect());
	shaders[shaderSelect]->setFloat("height_scale", mesh->getHeightScale());


	
	shaders[shaderSelect]->setInt("objMaterial.kdTexture", (mesh->getKD()==1) ? 1:4 );
	shaders[shaderSelect]->setFloat("objMaterial.IORin", mesh->getIORin());
	shaders[shaderSelect]->setFloat("objMaterial.IORout", mesh->getIORout());
	shaders[shaderSelect]->setInt("objMaterial.kreflect", mesh->getReflect());
	shaders[shaderSelect]->setInt("objMaterial.krefract", mesh->getRefract());
	shaders[shaderSelect]->setInt("objMaterial.knTexture", 3);
	shaders[shaderSelect]->setInt("objMaterial.kn", kn);
	shaders[shaderSelect]->setInt("objMaterial.kdepth", kdepth);
	shaders[shaderSelect]->setInt("objMaterial.depthTexture",  5);


	if (shaderSelect != 3) {//3 es orennayar
		shaders[shaderSelect]->setVec3("objMaterial.SpecularColor", geo->material.specular);
		shaders[shaderSelect]->setFloat("objMaterial.shininess", mesh->getShininess());

		shaders[shaderSelect]->setInt("objMaterial.ksTexture", (mesh->getKS()==1)?2:4 );
		shaders[shaderSelect]->setVec3("ambientLight.SpecularColor", Ambient->specular);
		shaders[shaderSelect]->setVec3("spotLight.SpecularColor", spotLight->specular);
	}

	shaders[shaderSelect]->setInt("spotLight.Active", spotLight->active);
	shaders[shaderSelect]->setFloat("spotLight.CuttOff", glm::cos(glm::radians(spotLight->cutOff)));
	shaders[shaderSelect]->setFloat("spotLight.OuterCutOff", glm::cos(glm::radians(spotLight->outerCutOff)));
	shaders[shaderSelect]->setVec3("spotLight.AmbientColor", spotLight->ambient);
	shaders[shaderSelect]->setVec3("spotLight.DifusseColor", spotLight->diffuse);
	shaders[shaderSelect]->setFloat("spotLight.Constant", spotLight->constant);
	shaders[shaderSelect]->setFloat("spotLight.Linear", spotLight->linear);
	shaders[shaderSelect]->setFloat("spotLight.Quadratic", spotLight->quadratic);

	int s;
	for (s = 0; s < pointLights.size(); s++) {
		shaders[shaderSelect]->setInt("pointLight[" + to_string(s) + "].Active", pointLights[s]->active);
		shaders[shaderSelect]->setVec3("pointLight[" + to_string(s) + "].AmbientColor", pointLights[s]->ambient);
		shaders[shaderSelect]->setVec3("pointLight[" + to_string(s) + "].DifusseColor", pointLights[s]->diffuse);
		shaders[shaderSelect]->setFloat("pointLight[" + to_string(s) + "].Constant", pointLights[s]->constant);
		shaders[shaderSelect]->setFloat("pointLight[" + to_string(s) + "].Linear", pointLights[s]->linear);
		shaders[shaderSelect]->setFloat("pointLight[" + to_string(s) + "].Quadratic", pointLights[s]->quadratic);
		if (shaderSelect != 3) {
			shaders[shaderSelect]->setVec3("pointLight[" + to_string(s) + "].SpecularColor", pointLights[s]->specular);
		}
	}


	ModelView = View * Model;
	shaders[shaderSelect]->setMat4("modelMatrix", Model);
	shaders[shaderSelect]->setMat4("projectMatrix", Projection);
	shaders[shaderSelect]->setMat4("viewMatrix", View);
	shaders[shaderSelect]->setMat4("modelViewMatrix", ModelView);
	shaders[shaderSelect]->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(Model))));
	shaders[shaderSelect]->setMat4("mvpMatrix", MVP);
	shaders[shaderSelect]->setVec3("viewPos", camera->Position);


}

/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */

 /**
  * Render Function
  * */

void activeTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDS->getTextureID(mesh->getTextureCubeMap()));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureIDS->getTextureID(geo->getTextureKD()));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureIDS->getTextureID(geo->getTextureKS()));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureIDS->getTextureID(geo->getTextureNM()));

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureIDS->getTextureID(1));//disable specular and diffuse texture

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textureIDS->getTextureID(geo->getTextureKDepth()));//depth texture
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

	glEnable(GL_FRAMEBUFFER_SRGB);
	for (i = 0; i < max; i++) {
		cont = meshes[i]->getGeometryLength();
		materialType = meshes[i]->getMaterialType();
		mesh = meshes[i];
		for (n = 0; n < cont; n++) {
			geo = mesh->getGeometry(n);
			Model = mesh->getMeshMatrix()* geo->getGeometryMatrix();
			switch (materialType)
			{
			case 0:
				shaders[1]->use();
				shaders[1]->setInt("skybox", 0);
				shaders[1]->setMat4("projectMatrix", Projection);
				shaders[1]->setMat4("viewMatrix", glm::mat4(glm::mat3(View)));
				break;
			case 1:
				activeShader(2);
				break;
			case 2:
				activeShader(3);
				break;
			case 3:
				activeShader(4);
				break;
			default:
				if (pointLights[i]->active == 1) {
					shaders[0]->use();
					shaders[0]->setVec3("objMaterial.AmbientColor", geo->material.ambient);
					shaders[0]->setVec3("objMaterial.DifusseColor", geo->material.diffuse);
					shaders[0]->setVec3("objMaterial.SpecularColor", geo->material.specular);
					glm::mat4 MVP = Projection * View* Model;
					shaders[0]->setMat4("mvpMatrix", MVP);
					glBindVertexArray(geo->getVAO());
					glDrawArrays(GL_TRIANGLES, 0, geo->getSizeVertex());
					glBindVertexArray(0);
				}
				break;
			}
			// Binds the vertex array to be drawn
			if (materialType == 0) {
				glDepthFunc(GL_LEQUAL);
				glBindVertexArray(geo->getVAO());
				activeTexture();

				// Renders the triangle gemotry
				glDrawArrays(GL_TRIANGLES, 0, geo->getSizeVertex());
				glBindVertexArray(0);
				// Swap the buffer
				glDepthFunc(GL_LESS);
			}
			else {
				if (materialType != -1) {

					glBindVertexArray(geo->getVAO());
					activeTexture();
					// Renders the triangle gemotry
					glDrawArrays(GL_TRIANGLES, 0, geo->getSizeVertex());
					glBindVertexArray(0);
				}
			}
		}
	}
	glDisable(GL_FRAMEBUFFER_SRGB);
	TwDraw();

	glfwSwapBuffers(window);

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

		updateUserInterface();

		// Renders everything
		render();

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
	TwTerminate();
	for (i = 0; i < textureIDS->getNumberTexture(); i++) {
		textureID = textureIDS->getTextureID(i);
		glDeleteTextures(1, &textureID);
	}
	// Deletes the vertex array from the GPU

	for (n = 0; n < max; n++) {
		cont = meshes[n]->getGeometryLength();
		for (i = 0; i < cont; i++) {
			geo = meshes[n]->getGeometry(i);
			VAO = geo->getVAO();
			glDeleteVertexArrays(1, &VAO);
			// Deletes the vertex object from the GPU
			VBOaux = geo->getVBO();
			VBO[0] = VBOaux[0];
			VBO[1] = VBOaux[1];
			VBO[2] = VBOaux[2];
			VBO[3] = VBOaux[3];
			VBO[4] = VBOaux[4];
			glDeleteBuffers(5, VBO);
			// Destroy the shader
		}
		meshes[n]->~Mesh();
	}
	// Stops the glfw program
	glfwTerminate();

	delete userInterface;
	delete shader;
	delete Ambient;
	delete spotLight;
	delete geo;
	delete mesh;
	delete camera;
	delete textureIDS;

	meshes.clear();
	shaders.clear();
	pointLights.clear();
	VBOaux.clear();


	return 0;
}



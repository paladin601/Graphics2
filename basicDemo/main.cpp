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


#include "ObjLoader.h"
#include "TextureLoader.h"
#include "Shader.h"
#include "Camera.h"
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
vector<Shader *> shaders;
Shader *shader;
Texture *textureIDS = new Texture();
DirectionalLight *Ambient;
SpotLight *spotLight;
vector<PointLight *> pointLights;
Geometry *geo;
Mesh *mesh;
vector<unsigned int>VBOaux;
vector<unsigned int>VBOS,VAOS;
Camera *camera = new Camera(glm::vec3(-8.0f, 4.0f, 5.0f));
glm::mat4 MVP, Projection, View, Model, ModelView;
glm::mat4 lightView;
glm::mat4 lightSpaceMatrix;
float near_plane = 0.01f, far_plane = 25.f;
glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */

bool meshPickedChange(int id) {
	return id == meshPicked;
}

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
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sets the ViewPort
	glViewport(0, 0, windowWidth, windowHeight);
	// Sets the clear color
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

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


Mesh* meshClone(Mesh* meshAux,glm::vec3 position,glm::vec3 scale,glm::vec3 rotation){
	int lengthGeo=meshAux->getGeometryLength();
	int in;
	Mesh* meshAct;
	Geometry*geoAct, *geoAux;
	meshAct=new Mesh();
	for(in=0;in<lengthGeo;in++){
		geoAct=new Geometry();
		geoAux = meshAux->getGeometry(in);
		geoAct->setVAO(geoAux->getVAO());
		geoAct->setName(geoAux->getName());
		geoAct->setSizeVertex(geoAux->getSizeVertex());
		meshAct->setGeometry(geoAct);
		meshAct->setTextureGeometry(in,geoAux->getTextureKD(),geoAux->getTextureKS(),geoAux->getTextureNM(),geoAux->getTextureKDepth());
	}
	meshAct->setRotate(rotation);
	meshAct->setScale(scale);
	meshAct->setTranslate(position);
	meshAct->setKD(meshAux->getKD());
	meshAct->setKS(meshAux->getKS());
	meshAct->setKN(meshAux->getKN());
	meshAct->setKDepth(meshAux->getKDepth());
	meshAct->setMaterialType(meshAux->getMaterialType());
	meshAct->setTextureCubeMap(meshAux->getTextureCubeMap());
	return meshAct;
}

void pushBuffers(Mesh* meshAux){
	int lengthGeo=meshAux->getGeometryLength();
	int in;
	for(in=0;in<lengthGeo;in++){
		VAOS.push_back(meshAux->getGeometry(in)->getVAO());	
		VBOaux=meshAux->getGeometry(in)->getVBO();
		VBOS.push_back(VBOaux[0]);
		VBOS.push_back(VBOaux[1]);
		VBOS.push_back(VBOaux[2]);
		VBOS.push_back(VBOaux[3]);
		VBOS.push_back(VBOaux[4]);
	}
}

bool init()
{
	// Initialize the window, and the glad components
	if (!initWindow() || !initGlad() )
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
	shader = new Shader("assets/shaders/depthShader.vert", "assets/shaders/depthShader.frag");
	shaders.push_back(shader);
	shader = new Shader("assets/shaders/depthQuadShader.vert", "assets/shaders/depthQuadShader.frag");
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

	// Loads the texture into the GPU
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
	textureIDS->setTexture(loaderTexture.load("assets/textures/boxNM.jpg"));
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
	//43
	textureIDS->setTexture(loaderTexture.load("assets/textures/window.png"));

	vector<Mesh *> meshesC;
	meshesC.push_back(loaderGeometry.load("assets/obj/pointlight.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/skybox.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/floor.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/cubito.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/personaje.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/pikachu.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/charmander.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/bulbasaur.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/brick1.obj"));
	meshesC.push_back(loaderGeometry.load("assets/obj/esfera.obj"));

	meshesC[1]->setMaterialType(0);
	meshesC[1]->setTextureCubeMap(0);

	meshesC[2]->setMaterialType(1);
	meshesC[2]->setTextureCubeMap(0);
	meshesC[2]->setTextureGeometry(0, 5, 5, 31, 33);
	meshesC[2]->setKD(0);
	meshesC[2]->setKS(0);
	meshesC[2]->setKN(0);

	meshesC[3]->setMaterialType(1);
	meshesC[3]->setTextureCubeMap(0);
	meshesC[3]->setTextureGeometry(0, 3, 4, 32, 33);

	meshesC[4]->setMaterialType(3);
	meshesC[4]->setTextureCubeMap(0);
	meshesC[4]->setTextureGeometry(0, 6, 7, 25, 33);
	meshesC[4]->setTextureGeometry(2, 8, 9, 26, 33);
	meshesC[4]->setTextureGeometry(3, 10, 11, 27, 33);
	meshesC[4]->setTextureGeometry(4, 12, 13, 28, 33);
	meshesC[4]->setTextureGeometry(5, 14, 15, 29, 33);
	meshesC[4]->setTextureGeometry(6, 16, 17, 30, 33);

	meshesC[5]->setMaterialType(1);
	meshesC[5]->setTextureCubeMap(0);
	meshesC[5]->setTextureGeometry(0, 18, 18, 32, 33);
	meshesC[5]->setTextureGeometry(1, 19, 19, 32, 33);
	meshesC[5]->setKN(0);

	meshesC[6]->setMaterialType(2);
	meshesC[6]->setTextureCubeMap(0);
	meshesC[6]->setTextureGeometry(0, 20, 20, 32, 33);
	meshesC[6]->setTextureGeometry(1, 21, 21, 32, 33);
	meshesC[6]->setTextureGeometry(2, 22, 22, 32, 33);
	meshesC[6]->setTextureGeometry(3, 22, 22, 32, 33);
	meshesC[6]->setKN(0);

	meshesC[7]->setMaterialType(3);
	meshesC[7]->setTextureCubeMap(0);
	meshesC[7]->setTextureGeometry(0, 24, 24, 32, 33);
	meshesC[7]->setTextureGeometry(1, 23, 23, 32, 33);
	meshesC[7]->setTextureGeometry(2, 24, 24, 32, 33);
	meshesC[7]->setKN(0);

	meshesC[8]->setMaterialType(1);
	meshesC[8]->setTextureCubeMap(0);
	meshesC[8]->setTextureGeometry(0, 34, 34, 35, 33);
	meshesC[8]->setKDepth(1);

	meshesC[9]->setMaterialType(1);
	meshesC[9]->setTextureCubeMap(0);
	meshesC[9]->setTextureGeometry(0, 34, 34, 35, 33);
	meshesC[9]->setKDepth(1);

	max = meshesC.size();
	// Loads all the geometry into the GPU
	for (i = 0; i < max; i++) {
		mesh = meshesC[i];
		buildGeometry();
	}
	int in;
	max=meshesC.size();
	for(in=0;in<max;in++){
		pushBuffers(meshesC[in]);
	}

	//pointlight model
	for(in=0;in<2;in++){
		meshes.push_back(meshClone(meshesC[0],glm::vec3(0.0f),glm::vec3(1.0f),glm::vec3(0.0f)));
	}
	//skybox
	meshes.push_back(meshClone(meshesC[1],glm::vec3(0.0f),glm::vec3(1.0f),glm::vec3(0.0f)));
	//floor
	meshes.push_back(meshClone(meshesC[2],glm::vec3(0.0f),glm::vec3(1.0f),glm::vec3(0.0f)));

	vector<glm::vec3> positions={
		glm::vec3(-3.0f, 0.61f, 0.0f),
		glm::vec3(-4.5f, 0.61f, 0.0f),
		glm::vec3(-6.0f, 0.61f, 0.0f),
	};

	vector<glm::vec3> rotations={
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
	};

	vector<glm::vec3> scales={
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
	};
	//cubes
	for(in=0;in<positions.size();in++){
		meshes.push_back(meshClone(meshesC[3],positions[in],scales[in],rotations[in]));
	}

	//pikachu
	meshes.push_back(meshClone(meshesC[5],glm::vec3(3.0f, 0.41f, -3.0f),glm::vec3(1.0f),glm::vec3(0.0f)));
	//charmander
	meshes.push_back(meshClone(meshesC[6],glm::vec3(4.0f, 0.55f, -3.0f),glm::vec3(1.0f),glm::vec3(0.0f)));
	//bulbasaur
	meshes.push_back(meshClone(meshesC[7],glm::vec3(5.0f, 0.27f, -3.0f),glm::vec3(1.0f),glm::vec3(0.0f)));

	positions.clear();

	positions={
		glm::vec3(-1.5f, 1.35f, -6.0f),
		glm::vec3(0.0f, 1.35f, -6.0f),
		glm::vec3(1.5f, 1.35f, -6.0f),
	};
	//personajes
	for(in=0;in<positions.size();in++){
		meshes.push_back(meshClone(meshesC[4],positions[in],scales[in],rotations[in]));
	}

	positions.clear();

	positions={
		glm::vec3(6.0f, 1.1f, -3.0f),
		glm::vec3(6.0f, 1.1f, -5.0f),
		glm::vec3(6.0f, 1.1f, -1.0f),
	};
	//brick red
	for(in=0;in<positions.size();in++){
		meshes.push_back(meshClone(meshesC[8],positions[in],scales[in],rotations[in]));
	}

	meshesC[8]->setTextureGeometry(0, 36, 36, 37, 38);
	positions.clear();
	rotations.clear();

	positions={
		glm::vec3(-4.0f, 1.1f, -9.0f),
		glm::vec3(0.0f, 1.1f, -9.0f),
	};
	rotations={
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.0f, 90.0f, 0.0f),
	};

	//plane with texture floor
	for(in=0;in<positions.size();in++){
		meshes.push_back(meshClone(meshesC[8],positions[in],scales[in],rotations[in]));
	}
	
	//plane with texture world
	meshesC[8]->setTextureGeometry(0, 39, 41, 40, 42);
	meshes.push_back(meshClone(meshesC[8],glm::vec3(4.0f, 1.1f, -9.0f),glm::vec3(1.0f),glm::vec3(0.0f, 90.0f, 0.0f)));

	//esphere
	meshes.push_back(meshClone(meshesC[9],glm::vec3(3.0f, 0.85f, 1.0f),glm::vec3(1.0f),glm::vec3(0.0f)));

	meshesC[8]->setTextureGeometry(0, 43, 43, 35, 33);
	meshesC[8]->setKDepth(0);
	meshesC[8]->setKN(0);
	positions.clear();
	rotations.clear();

	positions={
		glm::vec3(0.0f, 1.1f, -2.0f),
		glm::vec3(0.0f, 1.1f, 0.0f),
		glm::vec3(0.0f, 1.1f, 2.0f),
	};
	rotations={
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.0f, 90.0f, 0.0f),
	};
	//texture transparent
	for(in=0;in<positions.size();in++){
		meshes.push_back(meshClone(meshesC[8],positions[in],scales[in],rotations[in]));
	}

	max=meshes.size();
	meshesC.clear();
	positions.clear();
	rotations.clear();
	scales.clear();

	return true;
}


void activeShader(int shaderSelect) {
	int kn, kdepth;
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
	shaders[shaderSelect]->setInt("knActive", (kn == 1 || kdepth == 1) ? 1 : 0);



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
	shaders[shaderSelect]->setFloat("objMaterial.minLayer", mesh->getMinLayer());
	shaders[shaderSelect]->setFloat("objMaterial.maxLayer", mesh->getMaxLayer());



	shaders[shaderSelect]->setInt("objMaterial.kdTexture", (mesh->getKD() == 1) ? 1 : 4);
	shaders[shaderSelect]->setFloat("objMaterial.IORin", mesh->getIORin());
	shaders[shaderSelect]->setFloat("objMaterial.IORout", mesh->getIORout());
	shaders[shaderSelect]->setInt("objMaterial.kreflect", mesh->getReflect());
	shaders[shaderSelect]->setInt("objMaterial.krefract", mesh->getRefract());
	shaders[shaderSelect]->setInt("objMaterial.knTexture", 3);
	shaders[shaderSelect]->setInt("objMaterial.kn", kn);
	shaders[shaderSelect]->setInt("objMaterial.kdepth", kdepth);
	shaders[shaderSelect]->setInt("objMaterial.depthTexture", 5);
	shaders[shaderSelect]->setInt("shadowMapAL", 6);
	shaders[shaderSelect]->setInt("shadowMapSP", 7);
	shaders[shaderSelect]->setInt("shadowMapPL[0]", 8);
	shaders[shaderSelect]->setInt("shadowMapPL[1]", 9);


	if (shaderSelect != 3) {//3 es orennayar
		shaders[shaderSelect]->setVec3("objMaterial.SpecularColor", geo->material.specular);
		shaders[shaderSelect]->setFloat("objMaterial.shininess", mesh->getShininess());

		shaders[shaderSelect]->setInt("objMaterial.ksTexture", (mesh->getKS() == 1) ? 2 : 4);
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


}

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
	for (i = 0; i < textureIDS->getNumberTexture(); i++) {
		textureID = textureIDS->getTextureID(i);
		glDeleteTextures(1, &textureID);
	}

	// Deletes the vertex array from the GPU
	max=VAOS.size();
	for (n = 0; n < max; n++) {
		glDeleteVertexArrays(1, &VAOS[n]);
	}
	max=VBOS.size();
	for (n = 0; n < max; n++) {
		glDeleteBuffers(1, &VBOS[n]);
	}


	// Stops the glfw program
	glfwTerminate();
	delete shader;
	delete Ambient;
	delete spotLight;
	delete geo;
	delete mesh;
	delete camera;
	delete textureIDS;

	VAOS.clear();
	VBOS.clear();
	meshes.clear();
	shaders.clear();
	pointLights.clear();
	VBOaux.clear();


	return 0;
}
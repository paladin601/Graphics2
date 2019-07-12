
#pragma once
#include "Geometry.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureLoader.h"
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


using std::fstream;

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();
	static Mesh * load(string path);
};


#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>
#include <vector>
#include <string>
using namespace std;
using std::string;
using std::vector;

class TextureLoader
{
public:
	static unsigned int load(const char *path);
	static unsigned int loadCubeMap(vector<string> faces);
	TextureLoader();
	~TextureLoader();
};


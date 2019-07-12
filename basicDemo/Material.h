#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace std;

class Material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess,roughness;
	Material();
	~Material();
	void setMaterial(int);
	void setDefault();
	void setBronze();
	void setChrome();
	void setPlasticGreen();
};


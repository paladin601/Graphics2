#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class LightPropierties
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	int active;
	float constant,linear,quadratic;
	LightPropierties();
	~LightPropierties();
};


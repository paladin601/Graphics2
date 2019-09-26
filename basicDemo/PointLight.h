#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "LightPropierties.h"
class PointLight :public LightPropierties
{
public:
	glm::vec3 position;
	PointLight();
	~PointLight();
};

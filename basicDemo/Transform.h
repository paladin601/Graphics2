#pragma once
#include <iostream>
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\matrix_cross_product.hpp>
using namespace std;

class Transform
{
public:
	static glm::mat4 applyAllTransform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate);
	static glm::mat4 applyScale(glm::mat4, glm::vec3);
	static glm::mat4 applyRotate(glm::mat4, glm::vec3);
	static glm::mat4 applyTranslate(glm::mat4, glm::vec3);
	Transform();
	~Transform();
};


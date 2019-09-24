#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	ambient=glm::vec3(0,0,0);
	diffuse=specular=glm::vec3(1.0f);
	active=1;
	direction = glm::vec3(0.41f, -2.0f, -5.0f);
}


DirectionalLight::~DirectionalLight()
{
}

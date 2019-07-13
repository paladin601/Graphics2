#include "Material.h"



Material::Material()
{
}


Material::~Material()
{
}

void Material::setMaterial(int op)
{
	switch (op)
	{
	case 0:
		setBronze();
		break;
	case 1:
		setChrome();
		break;
	case 2:
		setPlasticGreen();
		break;
	default:
		setDefault();
		break;
	}
}

void Material::setDefault()
{
	ambient = glm::vec3(0.0f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);
	shininess = 128.0f;
	roughness = 0.20f;
}

void Material::setBronze()
{
	ambient = glm::vec3(0.2125f, 0.1275f, 0.054f);
	diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);
	specular = glm::vec3(0.393548f, 0.271906f, 0.166721f);
	shininess= 0.2f;
}

void Material::setChrome()
{
	ambient = glm::vec3(0.25f, 0.25f, 0.25f);
	diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
	shininess = 0.6f;
}

void Material::setPlasticGreen()
{
	ambient = glm::vec3(0.0f);
	diffuse = glm::vec3(0.1f, 0.35f, 0.1f);
	specular = glm::vec3(0.45f, 0.55f, 0.45f);
	shininess = 0.25f;
}

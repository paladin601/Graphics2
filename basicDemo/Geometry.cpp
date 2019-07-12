#include "Geometry.h"

void Geometry::setFace(glm::vec3 face)
{
	faces.push_back(face);
}

void Geometry::setNormal(glm::vec3 normal)
{
	normals.push_back(normal);
}

void Geometry::setUV(glm::vec2 uv)
{
	uvs.push_back(uv);
}

void Geometry::setVBO(unsigned int vbo)
{
	VBO.push_back(vbo);
}

vector<unsigned int> Geometry::getVBO()
{
	return VBO;
}

void Geometry::setVAO(unsigned int vao)
{
	VAO = vao;
}

int Geometry::getVAO()
{
	return VAO;
}

glm::mat4 Geometry::getGeometryMatrix()
{
	return transform.applyAllTransform(translate, scale, rotate);
}



vector<glm::vec3> Geometry::getFaces()
{
	return faces;
}

vector<glm::vec2> Geometry::getUVs()
{
	return uvs;
}

vector<glm::vec3> Geometry::getNormals()
{
	return normals;
}

Geometry::Geometry()
{
	kd = ks = 0;
	materialType = 0;
	translate = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotate = glm::vec3(0.0f);
	material.setMaterial(-1);
}


Geometry::~Geometry()
{
}

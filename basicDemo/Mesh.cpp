#include "Mesh.h"

Mesh::Mesh()
{
	cubeMap = false;
	translate = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotate = glm::vec3(0.0f);
	n = 0;
	materialType = -1;
	IORout = 1.0f;
	IORin = 1.52f;
}


Mesh::~Mesh()
{
	geo.clear();
}

Geometry * Mesh::getGeometry(int a)
{
	return geo[a];
}

void Mesh::setGeometry(Geometry *g)
{
	geo.push_back(g);
	n+=1;
}

int Mesh::getGeometryLength()
{
	return n;
}

glm::mat4 Mesh::getMeshMatrix()
{
	return transform.applyAllTransform(translate, scale, rotate);
}

int Mesh::getMaterialType()
{
	return materialType;
}

glm::vec3 Mesh::getTranslate()
{
	return translate;
}

glm::vec3 Mesh::getRotate()
{
	return rotate;
}

glm::vec3 Mesh::getScale()
{
	return scale;
}

float Mesh::getIORin()
{
	return IORin;
}

float Mesh::getIORout()
{
	return IORout;
}

void Mesh::setIORin(float in)
{
	IORin = in;
}

void Mesh::setIORout(float out)
{
	IORout = out;
}

void Mesh::setTextureGeometry(int geometry, int texturePosKd ,int texturePosKs)
{
	geo[geometry]->texturekd = texturePosKd;
	geo[geometry]->textureks = texturePosKs;
}

void Mesh::setMaterialType(int a)
{
	materialType = a;
}

void Mesh::setTranslate(glm::vec3 tr)
{
	translate = tr;
}

void Mesh::setRotate(glm::vec3 ro)
{
	rotate = ro;
}

void Mesh::setScale(glm::vec3 sc)
{
	scale = sc;
}

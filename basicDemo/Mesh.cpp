#include "Mesh.h"

Mesh::Mesh()
{
	shininess = 128.0f;
	roughness = 0.2f;
	reflect = refract = kd = ks  = 0;
	kn = 1;
	translate = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotate = glm::vec3(0.0f);
	textureCubeMap = 0;
	n = 0;
	materialType = -1;
	IORout = 1.0f;
	IORin = 1.52f;
	IReflect = 1.0f;
	FactorReflect = 0.8f;
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

int Mesh::getTextureCubeMap()
{
	return textureCubeMap;
}

int Mesh::getKS()
{
	return ks;
}

int Mesh::getKD()
{
	return kd;
}

int Mesh::getKN()
{
	return kn;
}

int Mesh::getReflect()
{
	return reflect;
}

int Mesh::getRefract()
{
	return refract;
}

float Mesh::getIReflect()
{
	return IReflect;
}

float Mesh::getFactorReflect()
{
	return FactorReflect;
}

float Mesh::getShininess()
{
	return shininess;
}

float Mesh::getRoughness()
{
	return roughness;
}

void Mesh::setShininess(float value)
{
	shininess = value;
}

void Mesh::setRoughness(float value)
{
	roughness = value;
}

void Mesh::setIReflect(float value)
{
	IReflect = value;
}

void Mesh::setFactorReflect(float value)
{
	FactorReflect = value;
}

void Mesh::setReflect(int active)
{
	reflect = active;
}

void Mesh::setRefract(int active)
{
	refract = active;
}

void Mesh::setKS(int coeficient)
{
	ks = coeficient;
}

void Mesh::setKD(int coeficient)
{
	kd = coeficient;
}

void Mesh::setKN(int coeficient)
{
	kn = coeficient;
}

void Mesh::setTextureCubeMap(int textureid)
{
	textureCubeMap = textureid;
}

void Mesh::setIORin(float in)
{
	IORin = in;
}

void Mesh::setIORout(float out)
{
	IORout = out;
}

void Mesh::setTextureGeometry(int geometry, int texturePosKd ,int texturePosKs, int texturePosKn)
{
	geo[geometry]->setTextureKD(texturePosKd);
	geo[geometry]->setTextureKS(texturePosKs);
	geo[geometry]->setTextureNM(texturePosKn);
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

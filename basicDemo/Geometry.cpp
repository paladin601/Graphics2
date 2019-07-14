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

void Geometry::setTangent(glm::vec3 tan)
{
	tangent.push_back(tan);
}

void Geometry::setBitangent(glm::vec3 bi)
{
	bitangent.push_back(bi);
}

void Geometry::setTextureNM(int textureid)
{
	textureNM = textureid;
}

void Geometry::setTextureKD(int textureid)
{
	textureKD = textureid;
}

void Geometry::setTextureKS(int textureid)
{
	textureKS = textureid;
}

void Geometry::setName(string namegeo)
{
	name = namegeo;
}

void Geometry::setSizeVertex(int size)
{
	sizeVertex = size;
}

void Geometry::setMaterialType(int type)
{
	materialType = type;
}

void Geometry::setTranslate(glm::vec3 trans)
{
	translate = trans;
}

void Geometry::setRotate(glm::vec3 rot)
{
	rotate = rot;
}

void Geometry::setScale(glm::vec3 sca)
{
	scale = sca;
}

string Geometry::getName()
{
	return name;
}

int Geometry::getSizeVertex()
{
	return sizeVertex;
}

int Geometry::getMaterialType()
{
	return materialType;
}

glm::vec3 Geometry::getTranslate()
{
	return translate;
}

glm::vec3 Geometry::getRotate()
{
	return rotate;
}

glm::vec3 Geometry::getScale()
{
	return scale;
}

int Geometry::getTextureNM()
{
	return textureNM;
}

int Geometry::getTextureKD()
{
	return textureKD;
}

int Geometry::getTextureKS()
{
	return textureKS;
}

vector<glm::vec3> Geometry::getTangent()
{
	return tangent;
}

vector<glm::vec3> Geometry::getBitangent()
{
	return bitangent;
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
	materialType = 0;
	translate = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotate = glm::vec3(0.0f);
	material.setMaterial(-1);
}


Geometry::~Geometry()
{
	faces.clear();
	normals.clear();
	tangent.clear();
	bitangent.clear();
	uvs.clear();
	VBO.clear();
}

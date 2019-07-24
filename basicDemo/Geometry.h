#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include"./Material.h"
#include"./Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\matrix_cross_product.hpp>
using namespace std;
using std::string;
using std::vector;

class Geometry
{
private:
	vector< glm::vec3 > faces, normals, tangent, bitangent;
	vector< glm::vec2 > uvs;
	vector<unsigned int> VBO;
	Transform transform;
	unsigned int VAO;
	int textureKD, textureKS, textureNM,textureDepth;
	string name;
	int sizeVertex;
	int materialType;
	glm::vec3 translate, rotate, scale;

public:
	Material material;
	
	void setVAO(unsigned int);
	void setVBO(unsigned int);
	void setFace(glm::vec3);
	void setUV(glm::vec2);
	void setNormal(glm::vec3);
	void setTangent(glm::vec3);
	void setBitangent(glm::vec3);
	void setTextureNM(int textureid);
	void setTextureKD(int textureid);
	void setTextureKS(int textureid);
	void setTextureKDepth(int textureid);


	void setName(string name);
	void setSizeVertex(int size);
	void setMaterialType(int type);
	void setTranslate(glm::vec3 trans);
	void setRotate(glm::vec3 rot);
	void setScale(glm::vec3 sca);
	
	

	string getName();
	int getSizeVertex();
	int getMaterialType();
	glm::vec3 getTranslate();
	glm::vec3 getRotate();
	glm::vec3 getScale();


	int getTextureNM();
	int getTextureKD();
	int getTextureKS();
	int getTextureKDepth();
	vector<glm::vec3> getTangent();
	vector<glm::vec3> getBitangent();
	unsigned int getVAO();
	vector<unsigned int> getVBO();
	glm::mat4 getGeometryMatrix();
	vector<glm::vec3> getFaces();
	vector<glm::vec2> getUVs();
	vector<glm::vec3> getNormals();

	Geometry();
	~Geometry();
};


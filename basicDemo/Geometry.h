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
	vector< glm::vec3 > faces;
	vector< glm::vec3 > normals;
	vector< glm::vec2 > uvs;
	vector<unsigned int> VBO;
	Transform transform;
	unsigned int VAO;

public:
	int kd, ks;
	int texturekd;
	int textureks;
	int sizeVertex;
	int materialType;
	string name;
	Material material;
	glm::vec3 translate, rotate, scale;
	void setFace(glm::vec3);
	void setNormal(glm::vec3);
	void setUV(glm::vec2);
	void setVBO(unsigned int);
	vector<unsigned int> getVBO();
	void setVAO(unsigned int);
	int getVAO();
	glm::mat4 getGeometryMatrix();
	vector<glm::vec3> getFaces();
	vector<glm::vec2> getUVs();
	vector<glm::vec3> getNormals();
	Geometry();
	~Geometry();
};


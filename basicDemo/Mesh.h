#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include"Geometry.h"
#include"Transform.h"
using namespace std;
using std::string;
using std::vector;


class Mesh
{
private:
	vector<Geometry *> geo;
	Transform transform;
	glm::mat4 meshMatrix;
	glm::vec3 translate, rotate, scale;
	int materialType;
	int n;
	float IORin, IORout;
	int kd, ks, kn, reflect, refract;
	int textureCubeMap;
public:
	Geometry* getGeometry(int);
	void setGeometry(Geometry*);
	int getGeometryLength();
	glm::mat4 getMeshMatrix();
	int getMaterialType();
	glm::vec3 getTranslate();
	glm::vec3 getRotate();
	glm::vec3 getScale();
	float getIORin();
	float getIORout();
	int getTextureCubeMap();
	int getKS();
	int getKD();
	int getKN();
	int getReflect();
	int getRefract();

	void setReflect(int active);
	void setRefract(int active);
	void setKS(int coeficient);
	void setKD(int coeficient);
	void setKN(int coeficient);

	void setTextureCubeMap(int textureid);
	void setIORin(float);
	void setIORout(float);
	void setTextureGeometry(int geometry, int texturePosKd, int texturePosKs,int texturePosKn);
	void setMaterialType(int);
	void setTranslate(glm::vec3);
	void setRotate(glm::vec3);
	void setScale(glm::vec3);


	Mesh();
	~Mesh();
};


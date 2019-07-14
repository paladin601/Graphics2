#pragma once

#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <AntTweakBar.h>
#include <iostream>
#include <string>

using namespace std;
using std::string;

typedef enum { BLINN_PHON, OREN_NAYAR ,COOK_TORRANCE} DeployTypeMaterial;

class CUserInterface
{
private:
	static CUserInterface * mInterface; //Holds the instance of the class
	TwBar *mUserInterface;
	int meshPicked;
	int geometryPicked;
	int geometryLength;
	string nameGeometry;
	float IORout, IORin;
	glm::vec3 positionMesh, scaleMesh, rotateMesh;
	glm::vec3 positionGeometry, scaleGeometry, rotateGeometry;
	glm::vec3 colorSpecularGeometry, colorAmbientGeometry, colorDiffuseGeometry;
	float shininess, roughness ,iReflect,factorReflect;
	DeployTypeMaterial mMaterial;
	glm::vec3 colorSpecularDL, colorAmbientDL, colorDiffuseDL;
	glm::vec3 DLAmbient;
	glm::vec3 colorSpecularSL, colorAmbientSL, colorDiffuseSL;
	float cutOffSL, outerCutOffSL, constantSL, linearSL, quadraticSL;
	bool activeDL, activeSL, activePL1, activePL2,kdMesh,ksMesh, knMesh ,reflectMesh,refractMesh;

	glm::vec3 colorSpecularPL1, colorAmbientPL1, colorDiffusePL1;
	glm::vec3 colorSpecularPL2, colorAmbientPL2, colorDiffusePL2;
	glm::vec3 positionPL1, positionPL2;
	float constantPL1, linearPL1, quadraticPL1;
	float constantPL2, linearPL2, quadraticPL2;

	CUserInterface();

public:
	///Method to obtain the only instance of the calls
	static CUserInterface * Instance();
	~CUserInterface();
	void reshape();

	void setIReflect(float value);
	float getIReflect();
	void setFactorReflect(float value);
	float getFactorReflect();



	int  getMeshMaterialPicked();
	void setMeshMaterialPicked(int);

	int getReflect();
	int getRefract();

	void setReflect(int active);
	void setRefract(int active);
	void setMeshKD(int coeficient);
	void setMeshKS(int coeficient);
	void setMeshKN(int coeficient);
	int getMeshKD();
	int getMeshKS();
	int getMeshKN();

	int getMeshPicked();
	int getGeometryPicked();
	glm::vec3 getMeshTranslate();
	glm::vec3 getMeshScale();
	glm::vec3 getMeshRotate();
	glm::vec3 getGeometryTranslate();
	glm::vec3 getGeometryScale();
	glm::vec3 getGeometryRotate();
	glm::vec3 getColorAmbientGeometry();
	glm::vec3 getColorDiffuseGeometry();
	glm::vec3 getColorSpecularGeometry();
	float getShininessGeometry();
	float getRoughnessGeometry();
	float getIORin();
	float getIORout();

	void setIORin(float);
	void setIORout(float);
	void setGeometryName(string a);
	void setGeometryPicked(int a);
	void setGeometryLength(int a);
	void setMeshTranslate(glm::vec3);
	void setMeshScale(glm::vec3);
	void setMeshRotate(glm::vec3);
	void setGeometryTranslate(glm::vec3);
	void setGeometryScale(glm::vec3);
	void setGeometryRotate(glm::vec3);	
	void setColorAmbientGeometry(glm::vec3);
	void setColorDiffuseGeometry(glm::vec3);
	void setColorSpecularGeometry(glm::vec3);
	void setShininessGeometry(float);
	void setRoughnessGeometry(float);


	bool getDLActive();
	glm::vec3 getDLLightAmbient();
	glm::vec3 getColorAmbientDL();
	glm::vec3 getColorDiffuseDL();
	glm::vec3 getColorSpecularDL();

	bool getSLActive();
	glm::vec3 getColorAmbientSL();
	glm::vec3 getColorDiffuseSL();
	glm::vec3 getColorSpecularSL();
	float getCutOffSL();
	float getOuterCutOffSL();
	float getConstantSL();
	float getLinearSL();
	float getQuadraticSL();

	bool getPL1Active();
	glm::vec3 getColorAmbientPL1();
	glm::vec3 getColorDiffusePL1();
	glm::vec3 getColorSpecularPL1();
	glm::vec3 getPositionPL1();
	float getConstantPL1();
	float getLinearPL1();
	float getQuadraticPL1();

	bool getPL2Active();
	glm::vec3 getColorAmbientPL2();
	glm::vec3 getColorDiffusePL2();
	glm::vec3 getColorSpecularPL2();
	glm::vec3 getPositionPL2();
	float getConstantPL2();
	float getLinearPL2();
	float getQuadraticPL2();


};

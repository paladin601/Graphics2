#include "UserInterface.h"

extern unsigned int windowWidth, windowHeight;

// Global static pointer used to ensure a single instance of the class.
CUserInterface * CUserInterface::mInterface = NULL;

/**
* Creates an instance of the class
*
* @return the instance of this class
*/
CUserInterface * CUserInterface::Instance()
{
	if (!mInterface)   // Only allow one instance of class to be generated.
		mInterface = new CUserInterface();

	return mInterface;
}

CUserInterface::CUserInterface()
{
	colorAmbientDL = glm::vec3(0.495f, 0.495f, 0.495f);
	colorDiffuseDL = glm::vec3(0.5f, 0.5f, 0.5f);
	colorSpecularDL = glm::vec3(1.0f, 1.0f, 1.0f);
	DLAmbient = glm::vec3(3.0f, -4.0f, -2.0f);
	activeDL = true;
	activeSL = false;
	activePL1 = false;
	activePL2 = false;
	colorAmbientSL = colorAmbientPL1 = colorAmbientPL2 = glm::vec3(0.495f);
	colorDiffuseSL = glm::vec3(1.0f);
	colorSpecularSL = glm::vec3(1.0f);

	colorDiffusePL1 = glm::vec3(1.0f, 0.0f, 0.0f);
	colorSpecularPL1 = glm::vec3(1.0f);
	colorDiffusePL2 = glm::vec3(0.0f, 1.0f, 0.0f);
	colorSpecularPL2 = glm::vec3(1.0f);

	positionPL1 = glm::vec3(3.0f, 1.5f, -0.5f);
	positionPL2 = glm::vec3(-3.0f, 1.5f, -0.5f);
	nameGeometry = "";
	cutOffSL = 12.5f;
	outerCutOffSL = 10.0f;

	constantPL2 = constantPL1 = constantSL = 1.0f;
	linearPL2 = linearPL1 = linearSL = 0.09f;
	quadraticPL2 = quadraticPL1 = quadraticSL = 0.032f;


	mUserInterface = TwNewBar("Tarea 1");
	TwDefine(" 'Tarea 1' color='0 0 0' alpha=200");
	TwDefine("'Tarea 1' refresh = '0.0001f'");
	TwDefine("'Tarea 1' resizable = true");
	TwDefine("'Tarea 1' fontresizable = false");
	TwDefine("'Tarea 1' movable = true");
	TwDefine("'Tarea 1' position = '20 20'");
	TwDefine("'Tarea 1' size = '220 320'");

	TwEnumVal DisplayMaterial[] = { { BLINN_PHON, "Blin Phong" },{ OREN_NAYAR, "Oren Nayar" },{ COOK_TORRANCE, "Cook Torrance" } };
	TwType DeployTwTypeMaterial = TwDefineEnum("DeployTypeMaterial", DisplayMaterial, 3);


	TwAddVarRW(mUserInterface, "Material Mesh", DeployTwTypeMaterial, &mMaterial, "label='Selected Material' group='Mesh'");

	TwAddVarRW(mUserInterface, "Select Mesh", TW_TYPE_INT8, &meshPicked, "label='Selected Mesh' group='Mesh' min=0 max=16");

	TwAddVarRW(mUserInterface, "I Reflect Mesh", TW_TYPE_FLOAT, &iReflect, "label='Indice Reflect' group='Reflect/Refract' step=0.01 min=0.5");
	TwAddVarRW(mUserInterface, "IORin X Mesh", TW_TYPE_FLOAT, &IORin, "label='IOR In' group='Reflect/Refract' step=0.01 min=0.1");
	TwAddVarRW(mUserInterface, "IORout Mesh", TW_TYPE_FLOAT, &IORout, "label='IOR Out' group='Reflect/Refract' step=0.01 min=0.1");

	TwAddVarRW(mUserInterface, "Height Mesh", TW_TYPE_FLOAT, &heightScale, "label='Height Scale' group='Parallax Mapping' step=0.001 min=0.001");

	TwAddVarRW(mUserInterface, "Shininess", TW_TYPE_FLOAT, &shininess, "label='Shininess' group='BDRF' step=0.01");
	TwAddVarRW(mUserInterface, "Roughness", TW_TYPE_FLOAT, &roughness, "label='Roughness' group='BDRF' step=0.01");
	TwAddVarRW(mUserInterface, "factor Reflect Mesh", TW_TYPE_FLOAT, &factorReflect, "label='Factor Reflect CT' group='BDRF' step=0.01 min=0.0");

	TwAddVarRW(mUserInterface, "kd Mesh", TW_TYPE_BOOLCPP, &kdMesh, "label='Diffuse texture' group='Active Mesh'");
	TwAddVarRW(mUserInterface, "ks Mesh", TW_TYPE_BOOLCPP, &ksMesh, "label='Specular texture' group='Active Mesh'");
	TwAddVarRW(mUserInterface, "kn Mesh", TW_TYPE_BOOLCPP, &knMesh, "label='Normal Mapping' group='Active Mesh'");
	TwAddVarRW(mUserInterface, "parallax Mesh", TW_TYPE_BOOLCPP, &kdepth, "label='Parallax Mapping' group='Active Mesh'");
	TwAddVarRW(mUserInterface, "reflect Mesh", TW_TYPE_BOOLCPP, &reflectMesh, "label='Reflect' group='Active Mesh'");
	TwAddVarRW(mUserInterface, "refract Mesh", TW_TYPE_BOOLCPP, &refractMesh, "label='Refract' group='Active Mesh'");






	TwAddVarRW(mUserInterface, "Position X Mesh", TW_TYPE_FLOAT, &positionMesh.x, "label='X' group='Position Mesh' step=0.01");
	TwAddVarRW(mUserInterface, "Position Y Mesh", TW_TYPE_FLOAT, &positionMesh.y, "label='Y' group='Position Mesh' step=0.01");
	TwAddVarRW(mUserInterface, "Position Z Mesh", TW_TYPE_FLOAT, &positionMesh.z, "label='Z' group='Position Mesh' step=0.01");
	TwAddVarRW(mUserInterface, "Scale XYZ  Mesh", TW_TYPE_FLOAT, &scaleMesh.x, "label='XYZ' group='Scale Mesh' min=0.1 step=0.1");
	TwAddVarRW(mUserInterface, "Rotate X   Mesh", TW_TYPE_FLOAT, &rotateMesh.x, "label='X' group='Rotate Mesh' min=0 max=360");
	TwAddVarRW(mUserInterface, "Rotate Y   Mesh", TW_TYPE_FLOAT, &rotateMesh.y, "label='Y' group='Rotate Mesh' min=0 max=360");
	TwAddVarRW(mUserInterface, "Rotate Z   Mesh", TW_TYPE_FLOAT, &rotateMesh.z, "label='Z' group='Rotate Mesh' min=0 max=360");



	TwAddVarRW(mUserInterface, "Select Geometry", TW_TYPE_INT8, &geometryPicked, "label='Selected Geometry' group='Geometry' min=0");
	TwAddVarRO(mUserInterface, "Name Geometry", TW_TYPE_STDSTRING, &nameGeometry, "label='Name Geometry' group='Geometry'");

	TwAddVarRW(mUserInterface, "Position X", TW_TYPE_FLOAT, &positionGeometry.x, "label='X' group='Position Geometry' step=0.01");
	TwAddVarRW(mUserInterface, "Position Y", TW_TYPE_FLOAT, &positionGeometry.y, "label='Y' group='Position Geometry' step=0.01");
	TwAddVarRW(mUserInterface, "Position Z", TW_TYPE_FLOAT, &positionGeometry.z, "label='Z' group='Position Geometry' step=0.01");
	TwAddVarRW(mUserInterface, "Scale XYZ", TW_TYPE_FLOAT, &scaleGeometry.x, "label='XYZ' group='Scale Geometry' min=0.1 step=0.1");
	TwAddVarRW(mUserInterface, "Rotate X", TW_TYPE_FLOAT, &rotateGeometry.x, "label='X' group='Rotate Geometry' min=0 max=360");
	TwAddVarRW(mUserInterface, "Rotate Y", TW_TYPE_FLOAT, &rotateGeometry.y, "label='Y' group='Rotate Geometry' min=0 max=360");
	TwAddVarRW(mUserInterface, "Rotate Z", TW_TYPE_FLOAT, &rotateGeometry.z, "label='Z' group='Rotate Geometry' min=0 max=360");

	TwAddVarRW(mUserInterface, "Color Ambient Geometry", TW_TYPE_COLOR3F, &colorAmbientGeometry, "label='Ambient Color' group='Material'");
	TwAddVarRW(mUserInterface, "Color Diffuse Geometry", TW_TYPE_COLOR3F, &colorDiffuseGeometry, "label='Diffuse Color' group='Material'");
	TwAddVarRW(mUserInterface, "Color Specular Geometry", TW_TYPE_COLOR3F, &colorSpecularGeometry, "label='Specular Color' group='Material'");



	TwAddVarRW(mUserInterface, "Active DL", TW_TYPE_BOOLCPP, &activeDL, "label='ON/OFF' group='Directional Light'");

	TwAddVarRW(mUserInterface, "Color Ambient DL", TW_TYPE_COLOR3F, &colorAmbientDL, "label='Ambient Color' group='ColorDL'");
	TwAddVarRW(mUserInterface, "Color Diffuse DL", TW_TYPE_COLOR3F, &colorDiffuseDL, "label='Diffuse Color' group='ColorDL'");
	TwAddVarRW(mUserInterface, "Color Specular DL", TW_TYPE_COLOR3F, &colorSpecularDL, "label='Specular Color' group='ColorDL'");

	TwAddVarRW(mUserInterface, "Direction DL", TW_TYPE_DIR3F, &DLAmbient, "label='Direction' ");

	TwAddVarRW(mUserInterface, "Active SL", TW_TYPE_BOOLCPP, &activeSL, "label='ON/OFF' group='Spot Light'");
	TwAddVarRW(mUserInterface, "Color Ambient SL", TW_TYPE_COLOR3F, &colorAmbientSL, "label='Ambient Color' group='ColorSL'");
	TwAddVarRW(mUserInterface, "Color Diffuse SL", TW_TYPE_COLOR3F, &colorDiffuseSL, "label='Diffuse Color' group='ColorSL'");
	TwAddVarRW(mUserInterface, "Color Specular SL", TW_TYPE_COLOR3F, &colorSpecularSL, "label='Specular Color' group='ColorSL'");
	TwAddVarRW(mUserInterface, "Cut Off SL", TW_TYPE_FLOAT, &cutOffSL, "label='Cut Off' group='AngleSL' min=1.0 max=120.0");
	TwAddVarRW(mUserInterface, "Outer Cut Off SL", TW_TYPE_FLOAT, &outerCutOffSL, "label='Outer Cut Off' group='AngleSL' min=0.1 max=60.0 help='This is sum of Cut off value + Outer Cut Off'");
	TwAddVarRW(mUserInterface, "Constant SL", TW_TYPE_FLOAT, &constantSL, "label='Constant' group='AttenuationSL' min=0.01 max=16.0 step=0.01");
	TwAddVarRW(mUserInterface, "Linear SL", TW_TYPE_FLOAT, &linearSL, "label='Linear' group='AttenuationSL' min=0.01 max=8.0 step=0.01");
	TwAddVarRW(mUserInterface, "Quadratic SL", TW_TYPE_FLOAT, &quadraticSL, "label='Quadratic' group='AttenuationSL' max=4.0 min=0.01 step=0.01");


	//point light
	TwAddVarRW(mUserInterface, "Active PL1", TW_TYPE_BOOLCPP, &activePL1, "label='ON/OFF' group='Point Light 1'");

	TwAddVarRW(mUserInterface, "Color Ambient PL1", TW_TYPE_COLOR3F, &colorAmbientPL1, "label='Ambient Color' group='ColorPL1'");
	TwAddVarRW(mUserInterface, "Color Diffuse PL1", TW_TYPE_COLOR3F, &colorDiffusePL1, "label='Diffuse Color' group='ColorPL1'");
	TwAddVarRW(mUserInterface, "Color Specular PL1", TW_TYPE_COLOR3F, &colorSpecularPL1, "label='Specular Color' group='ColorPL1'");

	TwAddVarRW(mUserInterface, "Constant PL1", TW_TYPE_FLOAT, &constantPL1, "label='Constant' group='AttenuationPL1' min=0.01 max=16.0 step=0.01");
	TwAddVarRW(mUserInterface, "Linear PL1", TW_TYPE_FLOAT, &linearPL1, "label='Linear' group='AttenuationPL1' min=0.01 max=8.0 step=0.01");
	TwAddVarRW(mUserInterface, "Quadratic PL1", TW_TYPE_FLOAT, &quadraticPL1, "label='Quadratic' group='AttenuationPL1' max=4.0 min=0.01 step=0.01");

	TwAddVarRW(mUserInterface, "Position X PL1", TW_TYPE_FLOAT, &positionPL1.x, "label='X' group='PositionPL1' step=0.01");
	TwAddVarRW(mUserInterface, "Position Y PL1", TW_TYPE_FLOAT, &positionPL1.y, "label='Y' group='PositionPL1' step=0.01");
	TwAddVarRW(mUserInterface, "Position Z PL1", TW_TYPE_FLOAT, &positionPL1.z, "label='Z' group='PositionPL1' step=0.01");


	TwAddVarRW(mUserInterface, "Active PL2", TW_TYPE_BOOLCPP, &activePL2, "label='ON/OFF' group='Point Light 2'");

	TwAddVarRW(mUserInterface, "Color Ambient PL2", TW_TYPE_COLOR3F, &colorAmbientPL2, "label='Ambient Color' group='ColorPL2'");
	TwAddVarRW(mUserInterface, "Color Diffuse PL2", TW_TYPE_COLOR3F, &colorDiffusePL2, "label='Diffuse Color' group='ColorPL2'");
	TwAddVarRW(mUserInterface, "Color Specular PL2", TW_TYPE_COLOR3F, &colorSpecularPL2, "label='Specular Color' group='ColorPL2'");

	TwAddVarRW(mUserInterface, "Constant PL2", TW_TYPE_FLOAT, &constantPL2, "label='Constant' group='AttenuationPL2' min=0.01 max=16.0 step=0.01");
	TwAddVarRW(mUserInterface, "Linear PL2", TW_TYPE_FLOAT, &linearPL2, "label='Linear' group='AttenuationPL2' min=0.01 max=8.0 step=0.01");
	TwAddVarRW(mUserInterface, "Quadratic PL2", TW_TYPE_FLOAT, &quadraticPL2, "label='Quadratic' group='AttenuationPL2' max=4.0 min=0.01 step=0.01");

	TwAddVarRW(mUserInterface, "Position X PL2", TW_TYPE_FLOAT, &positionPL2.x, "label='X' group='PositionPL2' step=0.01");
	TwAddVarRW(mUserInterface, "Position Y PL2", TW_TYPE_FLOAT, &positionPL2.y, "label='Y' group='PositionPL2' step=0.01");
	TwAddVarRW(mUserInterface, "Position Z PL2", TW_TYPE_FLOAT, &positionPL2.z, "label='Z' group='PositionPL2' step=0.01");


	TwDefine(" 'Tarea 1'/Mesh opened=true ");

	TwDefine(" 'Tarea 1'/'Reflect/Refract' group=Factor opened=false");
	TwDefine(" 'Tarea 1'/'Parallax Mapping' group=Factor opened=false");
	TwDefine(" 'Tarea 1'/'BDRF' group=Factor opened=false");
	TwDefine(" 'Tarea 1'/Factor group=Mesh opened=false");
	TwDefine(" 'Tarea 1'/'Position Mesh' group='Transform Mesh' opened=false");
	TwDefine(" 'Tarea 1'/'Scale Mesh'    group='Transform Mesh' opened=false");
	TwDefine(" 'Tarea 1'/'Rotate Mesh'   group='Transform Mesh' opened=false");
	TwDefine(" 'Tarea 1'/'Transform Mesh' group=Mesh opened=false");
	TwDefine(" 'Tarea 1'/'Active Mesh' group=Mesh opened=false");

	



	TwDefine(" 'Tarea 1'/Geometry opened=false group=Mesh ");
	TwDefine(" 'Tarea 1'/Material group=Geometry opened=false");
	TwDefine(" 'Tarea 1'/'Position Geometry' group=Transform opened=false");
	TwDefine(" 'Tarea 1'/'Scale Geometry' group=Transform opened=false");
	TwDefine(" 'Tarea 1'/'Rotate Geometry' group=Transform opened=false");
	TwDefine(" 'Tarea 1'/Transform group=Geometry opened=false");





	TwDefine(" 'Tarea 1'/'Directional Light' opened=false ");
	TwDefine(" 'Tarea 1'/ColorDL   group='Directional Light' opened=false  ");
	TwDefine(" 'Tarea 1'/'Direction DL'   group='Directional Light' ");

	TwDefine(" 'Tarea 1'/'Spot Light' opened=false ");
	TwDefine(" 'Tarea 1'/ColorSL   group='Spot Light'  opened=false");
	TwDefine(" 'Tarea 1'/AngleSL   group='Spot Light' opened=false");
	TwDefine(" 'Tarea 1'/AttenuationSL   group='Spot Light' opened=false");

	TwDefine(" 'Tarea 1'/'Point Light 1' opened=false ");
	TwDefine(" 'Tarea 1'/ColorPL1 group='Point Light 1' opened=false ");
	TwDefine(" 'Tarea 1'/PositionPL1 group='Point Light 1' opened=false ");
	TwDefine(" 'Tarea 1'/AttenuationPL1 group='Point Light 1' opened=false ");

	TwDefine(" 'Tarea 1'/'Point Light 2' opened=false ");
	TwDefine(" 'Tarea 1'/ColorPL2 group='Point Light 2' opened=false ");
	TwDefine(" 'Tarea 1'/PositionPL2 group='Point Light 2' opened=false ");
	TwDefine(" 'Tarea 1'/AttenuationPL2 group='Point Light 2' opened=false ");

}

CUserInterface::~CUserInterface()
{
}

void CUserInterface::reshape()
{
	TwWindowSize(windowWidth, windowHeight);
}

float CUserInterface::getHeightScale()
{
	return heightScale;
}

void CUserInterface::setHeightScale(float value)
{
	heightScale = value;
}

void CUserInterface::setIReflect(float value)
{
	iReflect = value;
}

float CUserInterface::getIReflect()
{
	return iReflect;
}

void CUserInterface::setFactorReflect(float value)
{
	factorReflect = value;
}

float CUserInterface::getFactorReflect()
{
	return factorReflect;
}

int CUserInterface::getMeshMaterialPicked()
{
	if (mMaterial == BLINN_PHON) return 1;
	if (mMaterial == OREN_NAYAR) return 2;
	if (mMaterial == COOK_TORRANCE) return 3;

}

void CUserInterface::setMeshMaterialPicked(int idMaterial)
{
	switch (idMaterial)
	{
	case 1:
		mMaterial = BLINN_PHON;
		break;
	case 2:
		mMaterial = OREN_NAYAR;
		break;
	case 3:
		mMaterial = COOK_TORRANCE;
		break;
	}
}

int CUserInterface::getReflect()
{
	if (reflectMesh) {
		return 1;
	}
	else {
		return 0;
	}
}

int CUserInterface::getRefract()
{
	if (refractMesh) {
		return 1;
	}
	else {
		return 0;
	}
}

void CUserInterface::setReflect(int active)
{
	if (active==1) {
		reflectMesh = true;
	}
	else {
		reflectMesh = false;
	}
}

void CUserInterface::setRefract(int active)
{
	if (active == 1) {
		refractMesh = true;
	}
	else {
		refractMesh = false;
	}
}


int CUserInterface::getMeshPicked()
{
	return meshPicked+3;
}

int CUserInterface::getGeometryPicked()
{
	if (geometryPicked >= geometryLength) {
		geometryPicked = geometryLength - 1;
	}
	return geometryPicked;
}

glm::vec3 CUserInterface::getMeshTranslate()
{
	return positionMesh;
}

glm::vec3 CUserInterface::getMeshScale()
{
	scaleMesh.y = scaleMesh.z = scaleMesh.x;
	return scaleMesh;
}

glm::vec3 CUserInterface::getMeshRotate()
{
	return rotateMesh;
}

glm::vec3 CUserInterface::getGeometryTranslate()
{
	return positionGeometry;
}

glm::vec3 CUserInterface::getGeometryScale()
{
	scaleGeometry.y = scaleGeometry.z = scaleGeometry.x;
	return scaleGeometry;
}

glm::vec3 CUserInterface::getGeometryRotate()
{
	return rotateGeometry;
}

glm::vec3 CUserInterface::getColorAmbientGeometry()
{
	return colorAmbientGeometry;
}

glm::vec3 CUserInterface::getColorDiffuseGeometry()
{
	return colorDiffuseGeometry;
}

glm::vec3 CUserInterface::getColorSpecularGeometry()
{
	return colorSpecularGeometry;
}

float CUserInterface::getShininessGeometry()
{
	return shininess;
}

float CUserInterface::getRoughnessGeometry()
{
	return roughness;
}

float CUserInterface::getIORin()
{
	return IORin;
}

float CUserInterface::getIORout()
{
	return IORout;
}

void CUserInterface::setIORin(float in)
{
	IORin = in;
}

void CUserInterface::setIORout(float out)
{
	IORout = out;
}

void CUserInterface::setGeometryName(string a)
{
	nameGeometry = a;
}

void CUserInterface::setGeometryPicked(int a)
{
	geometryPicked = a;
}

void CUserInterface::setGeometryLength(int a)
{
	geometryLength = a;
}

void CUserInterface::setMeshTranslate(glm::vec3 translate)
{
	positionMesh = translate;
}

void CUserInterface::setMeshScale(glm::vec3 scale)
{
	scaleMesh = scale;
}

void CUserInterface::setMeshRotate(glm::vec3 rotate)
{
	rotateMesh = rotate;
}

void CUserInterface::setGeometryTranslate(glm::vec3 translate)
{
	positionGeometry = translate;
}

void CUserInterface::setGeometryScale(glm::vec3 scale)
{
	scaleGeometry = scale;
}

void CUserInterface::setGeometryRotate(glm::vec3 rotate)
{
	rotateGeometry = rotate;
}

void CUserInterface::setColorAmbientGeometry(glm::vec3 ambient)
{
	colorAmbientGeometry = ambient;
}

void CUserInterface::setColorDiffuseGeometry(glm::vec3 diffuse)
{
	colorDiffuseGeometry = diffuse;
}

void CUserInterface::setColorSpecularGeometry(glm::vec3 specular)
{
	colorSpecularGeometry = specular;
}

void CUserInterface::setShininessGeometry(float shiny)
{
	shininess = shiny;
}

void CUserInterface::setRoughnessGeometry(float rough)
{
	roughness = rough;
}

bool CUserInterface::getDLActive()
{
	return activeDL;
}

glm::vec3 CUserInterface::getDLLightAmbient()
{
	return DLAmbient;
}

glm::vec3 CUserInterface::getColorAmbientDL()
{
	return colorAmbientDL;
}

glm::vec3 CUserInterface::getColorDiffuseDL()
{
	return colorDiffuseDL;
}

glm::vec3 CUserInterface::getColorSpecularDL()
{
	return colorSpecularDL;
}

bool CUserInterface::getSLActive()
{
	return activeSL;
}

glm::vec3 CUserInterface::getColorAmbientSL()
{
	return colorAmbientSL;
}

glm::vec3 CUserInterface::getColorDiffuseSL()
{
	return colorDiffuseSL;
}

glm::vec3 CUserInterface::getColorSpecularSL()
{
	return colorSpecularSL;
}

float CUserInterface::getCutOffSL()
{
	return cutOffSL;
}

float CUserInterface::getOuterCutOffSL()
{
	return cutOffSL + outerCutOffSL;
}

float CUserInterface::getConstantSL()
{
	return constantSL;
}

float CUserInterface::getLinearSL()
{
	return linearSL;
}

float CUserInterface::getQuadraticSL()
{
	return quadraticSL;
}

bool CUserInterface::getPL1Active()
{
	return activePL1;
}

glm::vec3 CUserInterface::getColorAmbientPL1()
{
	return colorAmbientPL1;
}

glm::vec3 CUserInterface::getColorDiffusePL1()
{
	return colorDiffusePL1;
}

glm::vec3 CUserInterface::getColorSpecularPL1()
{
	return colorSpecularPL1;
}

glm::vec3 CUserInterface::getPositionPL1()
{
	return positionPL1;
}

float CUserInterface::getConstantPL1()
{
	return constantPL1;
}

float CUserInterface::getLinearPL1()
{
	return linearPL1;
}

float CUserInterface::getQuadraticPL1()
{
	return quadraticPL1;
}

bool CUserInterface::getPL2Active()
{
	return activePL2;
}

glm::vec3 CUserInterface::getColorAmbientPL2()
{
	return colorAmbientPL2;
}

glm::vec3 CUserInterface::getColorDiffusePL2()
{
	return colorDiffusePL2;
}

glm::vec3 CUserInterface::getColorSpecularPL2()
{
	return colorSpecularPL2;
}

glm::vec3 CUserInterface::getPositionPL2()
{
	return positionPL2;
}

float CUserInterface::getConstantPL2()
{
	return constantPL2;
}

float CUserInterface::getLinearPL2()
{
	return linearPL2;
}

float CUserInterface::getQuadraticPL2()
{
	return quadraticPL2;
}

void CUserInterface::setMeshKD(int coeficient)
{
	if (coeficient == 1) {
		kdMesh = true;
	}
	else {
		kdMesh = false;
	}
}

void CUserInterface::setMeshKS(int coeficient)
{
	if (coeficient==1) {
		ksMesh = true;
	}
	else {
		ksMesh = false;
	}
}

void CUserInterface::setMeshKN(int coeficient)
{
	if (coeficient == 1) {
		knMesh = true;
	}
	else {
		knMesh = false;
	}
}

void CUserInterface::setMeshKDepth(int coeficient)
{
	if (coeficient == 1) {
		kdepth = true;
	}
	else {
		kdepth = false;
	}
}

int CUserInterface::getMeshKD()
{
	if (kdMesh) {
		return 1;
	}
	else {
		return 0;
	}
}

int CUserInterface::getMeshKS()
{
	if (ksMesh) {
		return 1;
	}
	else {
		return 0;
	}
}

int CUserInterface::getMeshKN()
{
	if (knMesh) {
		return 1;
	}
	else {
		return 0;
	}
}

int CUserInterface::getMeshKDepth()
{
	if (kdepth) {
		return 1;
	}
	else {
		return 0;
	}
}

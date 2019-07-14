#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out Data{
    vec3 vertexPos;
    vec3 normal;
    vec3 positionPL[2];
    vec3 positionSP;
    vec3 directionSP;
    vec3 directionAL;
    vec3 viewPos;
}dataOut;

uniform vec3 PositionPL[2];
uniform vec3 PositionSP;
uniform vec3 DirectionSP;
uniform vec3 DirectionAL;
uniform vec3 viewPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;
out vec2 vTexture;

void main() {

    vec3 T = normalize(mat3(modelMatrix) * aTangent);
    vec3 B = normalize(mat3(modelMatrix) * aBitangent);
    vec3 N = normalize(mat3(modelMatrix) * normal);
    mat3 TBN = mat3(T, B, N);

    dataOut.vertexPos=vec3(modelMatrix*vec4(position,1.f));
    dataOut.normal=normalMatrix*normal;
    dataOut.positionPL[0]= PositionPL[0];
    dataOut.positionPL[1]= PositionPL[1];
    dataOut.positionSP= PositionSP;
    dataOut.directionSP= DirectionSP;
    dataOut.directionAL= DirectionAL;
    dataOut.viewPos= viewPos;
    gl_Position=mvpMatrix*vec4(position,1.0f);
	vTexture=texture;
}

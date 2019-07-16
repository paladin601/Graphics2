#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;
//normalize in load
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
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;
out vec2 vTexture;
uniform int knActive;

void main() {

    vec3 aNormal=normal;
    vec3 atang=aTangent;
    vec3 abitang=aBitangent;
    mat3 model=mat3(modelMatrix);
    vec3 T = normalize(model * atang);
    vec3 N = normalize(model * aNormal);
    vec3 B = normalize(model*abitang);

    mat3 TBN=mat3(1.0f);
    if(knActive==1){
        TBN = transpose(mat3(T, B, N));
    }

    dataOut.normal= normalMatrix*normal;
    dataOut.vertexPos= TBN*vec3(modelMatrix*vec4(position,1.f));
    dataOut.positionPL[0]= TBN*PositionPL[0];
    dataOut.positionPL[1]= TBN*PositionPL[1];
    dataOut.positionSP= TBN*PositionSP;
    dataOut.directionSP= TBN*DirectionSP;
    dataOut.directionAL= TBN*DirectionAL;
    dataOut.viewPos= TBN*viewPos;
    gl_Position=mvpMatrix*vec4(position,1.0f);
	vTexture=texture;
}

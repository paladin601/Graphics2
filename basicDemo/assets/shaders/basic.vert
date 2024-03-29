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
    vec4 vertexPosLightAL;
    vec4 vertexPosLightSP;
    vec4 vertexPosLightPL[2];
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
uniform mat4 lightSpaceMatrixAL;
uniform mat4 lightSpaceMatrixSP;
uniform mat4 lightSpaceMatrixPL[2];

void main() {

    vec3 aNormal=normalize(normal);
    vec3 atang=normalize(aTangent);
    vec3 abitang=normalize(aBitangent);
    mat3 model=mat3(modelMatrix);
    vec3 T = normalize(model * atang);
    vec3 N = normalize(model * aNormal);
    T =normalize(T - N * dot(N, T));
    //vec3 B = normalize(model*abitang);
    vec3 B = normalize(cross(T,N));
    

    mat3 TBN=mat3(1.0f);
    if(knActive==1){
        TBN = transpose(mat3(T, B, N));
    }
    vec3 vertex=vec3(modelMatrix*vec4(position,1.f));
    dataOut.normal= normalMatrix*normal;
    dataOut.vertexPosLightAL=lightSpaceMatrixAL* vec4(vertex,1.0f);
    dataOut.vertexPosLightSP=lightSpaceMatrixSP* vec4(vertex,1.0f);
    dataOut.vertexPosLightPL[0]=lightSpaceMatrixPL[0]* vec4(vertex,1.0f);
    dataOut.vertexPosLightPL[1]=lightSpaceMatrixPL[1]* vec4(vertex,1.0f);
    dataOut.vertexPos= TBN*vertex;
    dataOut.positionPL[0]= TBN*PositionPL[0];
    dataOut.positionPL[1]= TBN*PositionPL[1];
    dataOut.positionSP= TBN*PositionSP;
    dataOut.directionSP= TBN*DirectionSP;
    dataOut.directionAL= TBN*DirectionAL;
    dataOut.viewPos= TBN*viewPos;
    gl_Position=mvpMatrix*vec4(position,1.0f);
	vTexture=texture;
}

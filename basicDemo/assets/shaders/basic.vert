#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out Data{
    vec3 vertexPos;
    vec3 normal;
}dataOut;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;
out vec2 vTexture;

void main() {

    vec3 T = normalize(vec3(modelMatrix * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(modelMatrix * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(modelMatrix * vec4(normal,    0.0)));
    mat3 TBN = mat3(T, B, N);

    dataOut.vertexPos=vec3(modelMatrix*vec4(position,1.f));

    dataOut.normal=normalMatrix*normal;
    gl_Position=mvpMatrix*vec4(position,1.0f);
	vTexture=texture;
}

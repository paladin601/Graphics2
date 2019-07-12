#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

out Data{
    vec3 vertexPos;
    vec3 normal;
}dataOut;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;
out vec2 vTexture;

void main() {

    dataOut.vertexPos=vec3(modelMatrix*vec4(position,1.f));

    dataOut.normal=normalMatrix*normal;
    gl_Position=mvpMatrix*vec4(position,1.0f);
	vTexture=texture;
}

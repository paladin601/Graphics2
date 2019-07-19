#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

// Uniforms
// -----------------------
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;
out vec2 vTexture;

void main()
{
    vTexture=vTex;
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vPos,1.0f);
}
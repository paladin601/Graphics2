#version 330 core
in vec2 texCoords;

// Texture 
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(texture(gAlbedoSpec, texCoords));
}
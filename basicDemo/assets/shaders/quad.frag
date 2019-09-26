#version 330 core
in vec2 texCoords;

// Texture 
uniform sampler2D text;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(texture(text,texCoords).rgb,1.0f);
}
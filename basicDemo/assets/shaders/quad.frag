#version 330 core
in vec2 texCoords;

// Texture 
uniform sampler2D text;

// Fragment Color
out vec4 color;

void main()
{
    color = texture(text,texCoords);
}
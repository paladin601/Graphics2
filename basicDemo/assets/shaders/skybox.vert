#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projectMatrix;
uniform mat4 viewMatrix;

void main()
{
    TexCoords = aPos;
    vec4 pos = projectMatrix * viewMatrix * vec4(aPos, 1.0);
    
    gl_Position = pos.xyww;
}  
#version 330 core

in vec3 posShifted;

out vec4 FragColor;

void main()
{
    FragColor = vec4(posShifted,1.0f);
}

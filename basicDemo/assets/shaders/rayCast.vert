#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 posShifted;

void main()
{
	posShifted = pos+0.5f;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
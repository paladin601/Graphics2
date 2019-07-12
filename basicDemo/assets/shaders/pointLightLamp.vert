#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

uniform mat4 mvpMatrix;

void main() {
    gl_Position=mvpMatrix*vec4(position,1.0f);
}

#version 330 core
in vec2 textCoord;

//Texture
uniform sampler3D texture;
uniform float step;

out vec4 FragColor;

void main()
{
    vec3 coordUV=vec3(textCoord.x,textCoord.y,step);
    FragColor = vec4(vec3(texture(texture,coordUV).r),1.0f);
}
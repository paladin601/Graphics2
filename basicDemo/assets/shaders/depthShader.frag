#version 330 core

uniform sampler2D kd;
in vec2 vTexture;

void main()
{             
    if(texture(kd,vTexture).a<0.5f)
        discard;

}  
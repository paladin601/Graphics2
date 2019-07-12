#version 330 core
out vec4 fragColor;

struct Material{
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
};

uniform Material objMaterial;

void main() {   
    vec3 color = objMaterial.AmbientColor*0.1+objMaterial.DifusseColor*0.45+objMaterial.SpecularColor*0.45;
    fragColor= vec4(color, 1.0f);
}

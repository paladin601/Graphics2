#version 330 core
in vec2 texCoords;

// Texture 
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct AmbientLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    vec3 directionAL;
};
uniform AmbientLight ambientLight;

uniform vec3 viewPos;

float blinnPhongVal(vec3 normal, vec3 lightDir, vec3 viewDir){
    vec3 H=normalize(lightDir+viewDir);
    return pow(clamp(dot(H,normal),0.0f,1.0f),64);
}

vec3 AmbientLightCon(AmbientLight light,vec3 normal, vec3 lightDir, vec3 viewDir){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor * 0.1*texture(gAlbedoSpec,texCoords).rgb;
    float diff=clamp(dot(normal,lightDir),0.0f,1.0f);
    vec3 diffuse=light.DifusseColor*diff*texture(gAlbedoSpec,texCoords).rgb;
    
    float spec=blinnPhongVal(normal,lightDir,viewDir);
    vec3 ks=vec3(texture(gAlbedoSpec,texCoords).a);
    vec3 specular=light.SpecularColor*spec*ks;
    Contribution=(ambient+diffuse)+(specular);
    return Contribution;
}

// Fragment Color
out vec4 color;

void main()
{
    vec3 vertexPos=texture(gPosition,texCoords).rgb;
    vec3 viewDir=normalize(viewPos-vertexPos);
    vec3 normal=texture(gNormal,texCoords).rgb;
    vec3 lightContribution=AmbientLightCon(ambientLight,normal,normalize(-ambientLight.directionAL),viewDir);
    color = vec4(lightContribution,1.0f);
}
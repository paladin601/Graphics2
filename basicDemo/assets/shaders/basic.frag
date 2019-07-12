#version 330 core
out vec4 fragColor;

in Data{
    vec3 vertexPos;
    vec3 normal;
}dataIn;
in vec2 vTexture;

struct AmbientLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    vec3 Direction;
};

struct SpotLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    vec3 Direction;
    vec3 Position;
    float CuttOff;
    float OuterCutOff;
    float Constant;
    float Linear;
    float Quadratic;
};

struct PointLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
};
 
#define NR_POINT_LIGHTS 2

uniform AmbientLight ambientLight;
uniform SpotLight spotLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];

struct Material{
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    float shininess;
    float roughness;
    int kd;
    int ks;
    sampler2D kdTexture;
    sampler2D ksTexture;
};

uniform Material objMaterial;

uniform vec3 viewPos;
uniform float shininess;

float Attenuation(float constant,float linear,float quadratic, float distance){
    return 1.0/(constant+linear * distance + quadratic * (distance*distance));
}

float blinnPhongVal(vec3 normal, vec3 lightDir, vec3 viewDir){
    vec3 H=normalize(lightDir+viewDir);
    return pow(max(dot(H,normal),0.0f),objMaterial.shininess);
}

vec3 PointLightCon(PointLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd,vec3 ks){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    float diff=max(dot(normal,lightDir),0.0f);
    vec3 diffuse=light.DifusseColor*(diff *kd);

    float spec=blinnPhongVal(normal,lightDir,viewDir);
    vec3 specular=light.SpecularColor*(spec * ks);
    float distance =length(light.Position-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);
    Contribution=(ambient+diffuse+specular)*attenuation;
    return Contribution;
}

vec3 AmbientLightCon(AmbientLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd,vec3 ks){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor * objMaterial.AmbientColor;
    float diff=max(dot(normal,lightDir),0.0f);
    vec3 diffuse=light.DifusseColor*(diff * kd);
    
    float spec=blinnPhongVal(normal,lightDir,viewDir);
    vec3 specular=light.SpecularColor*(spec  * ks);
    Contribution=ambient+diffuse+specular;
    return Contribution;
}

vec3 SpotLightCon(SpotLight light, vec3 normal, vec3 lightDir,vec3 viewDir,vec3 kd,vec3 ks){
    vec3 Contribution;
    
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    float diff=max(dot(normal,lightDir),0.0f);
    vec3 diffuse=light.DifusseColor*(diff *kd);

    float spec=blinnPhongVal(normal,lightDir,viewDir);
    vec3 specular=light.SpecularColor*(spec * ks);
    float distance =length(light.Position-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);
    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.CuttOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    Contribution=(ambient+diffuse+specular)*attenuation *intensity;
    return Contribution;

}

void main() {   
    vec3 normal=normalize(dataIn.normal);
    vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
    vec3 lightContribution=vec3(0.0f);
    vec3 kd=objMaterial.DifusseColor;
    if(objMaterial.kd==1){
        if(texture2D(objMaterial.kdTexture, vTexture).a < 0.1)
            discard;
        kd=texture2D(objMaterial.kdTexture, vTexture).rgb;
    }
    vec3 ks=objMaterial.SpecularColor;
    if(objMaterial.ks==1){
        if(texture2D(objMaterial.ksTexture, vTexture).a < 0.1)
            discard;
        ks=texture2D(objMaterial.ksTexture, vTexture).rgb;
    }
    if(ambientLight.Active == 1){
        lightContribution=AmbientLightCon(ambientLight,normal,normalize(-ambientLight.Direction),viewDir,kd,ks);
    }
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        if(pointLight[i].Active==1){
            lightContribution+=PointLightCon(pointLight[i],normal,normalize(pointLight[i].Position-dataIn.vertexPos),viewDir,kd,ks);
        }
    }
    if(spotLight.Active==1){
        lightContribution+=SpotLightCon(spotLight,normal,normalize(spotLight.Position-dataIn.vertexPos),viewDir,kd,ks);
    }


    fragColor= vec4(lightContribution, 1.0f);
}

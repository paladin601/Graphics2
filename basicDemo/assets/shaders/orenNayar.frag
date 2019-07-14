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
    vec3 Direction;
};

struct SpotLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
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
    float shininess;
    float roughness;
    float IORin;
    float IORout;
    int kreflect;
    int krefract;
    int kd;
    sampler2D kdTexture;
};

uniform Material objMaterial;

uniform vec3 viewPos;

uniform samplerCube skybox;


float Attenuation(float constant,float linear,float quadratic, float distance){
    return 1.0/(constant+linear * distance + quadratic * (distance*distance));
}

float orenNayarVal(vec3 normal, vec3 lightDir, vec3 viewDir){
    float VdotN=max(dot(viewDir,normal),0.0f);
    float LdotN=max(dot(lightDir,normal),0.0f);
    vec3 VprodN=viewDir-normal*VdotN;
    vec3 LprodN=lightDir-normal*LdotN;

    float prodVLN=max(dot(normalize(VprodN),normalize(LprodN)),0.0f);

    float alpha=max(acos(LdotN),acos(VdotN));
    float beta=min(acos(LdotN),acos(VdotN));
    float roughness2=objMaterial.roughness*objMaterial.roughness;
    float A=1.0f-0.5f*(roughness2/(roughness2+0.57));
    float B=0.45f*roughness2/(roughness2+0.09f);
    return (A+(prodVLN*B*sin(alpha)*tan(beta)));
}

vec3 PointLightCon(PointLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;

    float VdotN=max(dot(viewDir,normal),0.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    
    vec3 diffuse=VdotN*light.DifusseColor*(diff *kd);

    float distance =length(light.Position-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);

    Contribution=(ambient+diffuse)*attenuation;
    return Contribution;
}

vec3 AmbientLightCon(AmbientLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd){
    vec3 Contribution;

    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    
    float VdotN=max(dot(viewDir,normal),0.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    vec3 diffuse=VdotN*light.DifusseColor*(diff * kd);

    Contribution=(ambient+diffuse);
    return Contribution;
}

vec3 SpotLightCon(SpotLight light, vec3 normal, vec3 lightDir,vec3 viewDir,vec3 kd){
    vec3 Contribution;
    
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;

    float VdotN=max(dot(viewDir,normal),0.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    vec3 diffuse=VdotN*light.DifusseColor*(diff *kd);


    float distance =length(light.Position-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);
    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.CuttOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
            
    
    Contribution=(ambient+diffuse)*attenuation *intensity;
    return Contribution;

}

void main() {   
    vec3 normal=normalize(dataIn.normal);
    vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
    vec3 lightContribution=vec3(0.0f);


    vec3 kd=objMaterial.DifusseColor;
    if(texture2D(objMaterial.kdTexture, vTexture).a < 0.1)
        discard;
    kd*=texture2D(objMaterial.kdTexture, vTexture).rgb;


    if(ambientLight.Active == 1){
        lightContribution=AmbientLightCon(ambientLight,normal,normalize(-ambientLight.Direction),viewDir,kd);
    }
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        if(pointLight[i].Active==1){
            lightContribution+=PointLightCon(pointLight[i],normal,normalize(pointLight[i].Position-dataIn.vertexPos),viewDir,kd);
        }
    }
    if(spotLight.Active==1){
        lightContribution+=SpotLightCon(spotLight,normal,normalize(spotLight.Position-dataIn.vertexPos),viewDir,kd);
    }

    vec4 r=vec4(1.0f);
    if(objMaterial.krefract==1){
        viewDir=normalize(dataIn.vertexPos-viewPos);
        float RefractInd= objMaterial.IORout/objMaterial.IORin;
        vec3 R = refract(viewDir, normal,RefractInd);
        r=vec4(texture(skybox,R).rgb,1.0f); 
    }
    if(objMaterial.kreflect==1){
        viewDir=normalize(dataIn.vertexPos-viewPos);
        vec3 R = reflect(viewDir, normal);
        r=vec4(texture(skybox,R).rgb,1.0f);
    }


    fragColor= vec4(lightContribution, 1.0f)*r;
}

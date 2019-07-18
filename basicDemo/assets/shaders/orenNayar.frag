#version 330 core
out vec4 fragColor;

in Data{
    vec3 vertexPos;
    vec3 normal;
    vec3 positionPL[2];
    vec3 positionSP;
    vec3 directionSP;
    vec3 directionAL;
    vec3 viewPos;
}dataIn;
in vec2 vTexture;

struct AmbientLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
};

struct SpotLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
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
    float IReflect;
    float IORin;
    float IORout;
    float minLayer;
    float maxLayer;
    int kreflect;
    int krefract;
    int kn;
    int kdepth;
    sampler2D kdTexture;
    sampler2D knTexture;
    sampler2D depthTexture;
};

uniform Material objMaterial;
uniform float height_scale=0.1f;
uniform samplerCube skybox;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float numLayers = mix(objMaterial.maxLayer, objMaterial.minLayer, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2  currentTexCoords = texCoords;
    float height =  texture(objMaterial.depthTexture, currentTexCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = p / numLayers;
      
    while(currentLayerDepth < height)
    {
        currentTexCoords -= deltaTexCoords;
        height = texture(objMaterial.depthTexture, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = height - currentLayerDepth;
    float beforeDepth = texture(objMaterial.depthTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
} 

float Attenuation(float constant,float linear,float quadratic, float distance){
    return 1.0/(constant+linear * distance + quadratic * (distance*distance));
}

float orenNayarVal(vec3 normal, vec3 lightDir, vec3 viewDir){
    float VdotN=clamp(dot(viewDir,normal),0.0f,1.0f);
    float LdotN=clamp(dot(lightDir,normal),0.0f,1.0f);
    vec3 VprodN=viewDir-normal*VdotN;
    vec3 LprodN=lightDir-normal*LdotN;

    float prodVLN=clamp(dot(normalize(VprodN),normalize(LprodN)),0.0f,1.0f);

    float alpha=max(acos(LdotN),acos(VdotN));
    float beta=min(acos(LdotN),acos(VdotN));
    float roughness2=objMaterial.roughness*objMaterial.roughness;
    float A=1.0f-0.5f*(roughness2/(roughness2+0.57));
    float B=0.45f*roughness2/(roughness2+0.09f);
    return (A+(prodVLN*B*sin(alpha)*tan(beta)));
}

vec3 PointLightCon(PointLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd,int i){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;

    float VdotN=clamp(dot(viewDir,normal),0.0f,1.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    
    vec3 diffuse=VdotN*light.DifusseColor*(diff *kd);

    float distance =length(dataIn.positionPL[i]-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);

    Contribution=(ambient+diffuse)*attenuation;
    return Contribution;
}

vec3 AmbientLightCon(AmbientLight light,vec3 normal, vec3 lightDir, vec3 viewDir,vec3 kd){
    vec3 Contribution;

    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    
    float VdotN=clamp(dot(viewDir,normal),0.0f,1.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    vec3 diffuse=VdotN*light.DifusseColor*(diff * kd);

    Contribution=(ambient+diffuse);
    return Contribution;
}

vec3 SpotLightCon(SpotLight light, vec3 normal, vec3 lightDir,vec3 viewDir,vec3 kd){
    vec3 Contribution;
    
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;

    float VdotN=clamp(dot(viewDir,normal),0.0f,1.0f);

    float diff=orenNayarVal(normal,lightDir,viewDir);
    vec3 diffuse=VdotN*light.DifusseColor*(diff *kd);


    float distance =length(dataIn.positionSP-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);
    float theta = dot(lightDir, normalize(-dataIn.directionSP)); 
    float epsilon = light.CuttOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
            
    
    Contribution=(ambient+diffuse)*attenuation *intensity;
    return Contribution;

}

void main() {   
    vec3 viewDir=normalize(dataIn.viewPos-dataIn.vertexPos);

    vec2 texCoords = vTexture;
    if(objMaterial.kdepth==1){
        texCoords=ParallaxMapping(texCoords,  viewDir);
    }

    vec3 normal=normalize(dataIn.normal);
    if(objMaterial.kn==1){
        normal= texture(objMaterial.knTexture, texCoords).rgb;
        normal=normalize(normal*2.0f-vec3(1.0f));
    }

    
    vec3 lightContribution=vec3(0.0f);


    vec3 kd=objMaterial.DifusseColor;
    if(texture2D(objMaterial.kdTexture, texCoords).a < 0.1)
        discard;
    kd*=texture2D(objMaterial.kdTexture, texCoords).rgb;


    if(ambientLight.Active == 1){
        lightContribution=AmbientLightCon(ambientLight,normal,normalize(-dataIn.directionAL),viewDir,kd);
    }
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        if(pointLight[i].Active==1){
            lightContribution+=PointLightCon(pointLight[i],normal,normalize(dataIn.positionPL[i]-dataIn.vertexPos),viewDir,kd,i);
        }
    }
    if(spotLight.Active==1){
        lightContribution+=SpotLightCon(spotLight,normal,normalize(dataIn.positionSP-dataIn.vertexPos),viewDir,kd);
    }

    vec3 refr=vec3(1.0f);
    if(objMaterial.krefract==1){
        viewDir=normalize(dataIn.vertexPos-dataIn.viewPos);
        float RefractInd= objMaterial.IORout/objMaterial.IORin;
        vec3 R = refract(viewDir, normal,RefractInd);
        refr=texture(skybox,R).rgb; 
    }
    vec3 refl=vec3(1.0f);
    if(objMaterial.kreflect==1){
        viewDir=normalize(dataIn.vertexPos-dataIn.viewPos);
        vec3 R = reflect(viewDir, normal);
        refl=texture(skybox,R).rgb;
        refl=objMaterial.IReflect*refl;
    }


    fragColor= vec4(lightContribution*refl*refr, 1.0f);    
}

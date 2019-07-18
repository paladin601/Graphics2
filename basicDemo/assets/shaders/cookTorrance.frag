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
    vec3 SpecularColor;
};

struct SpotLight{
    int Active;
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
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
    float Constant;
    float Linear;
    float Quadratic;
};
 
#define NR_POINT_LIGHTS 2
uniform float factReflec;

uniform AmbientLight ambientLight;
uniform SpotLight spotLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];

struct Material{
    vec3 AmbientColor;
    vec3 DifusseColor;
    vec3 SpecularColor;
    float shininess;
    float roughness;
    float IORin;
    float IORout;
    float IReflect;
    float minLayer;
    float maxLayer;
    int kreflect;
    int krefract;
    int kn;
    int kdepth;
    sampler2D kdTexture;
    sampler2D ksTexture;
    sampler2D knTexture;
    sampler2D depthTexture;
};

uniform Material objMaterial;
uniform float height_scale=0.1f;
uniform samplerCube skybox;

float PI=3.14159265f;

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


float cookTorranceVal(vec3 normal, vec3 lightDir, vec3 viewDir){
    vec3 H = normalize(lightDir+viewDir);
    float LdotN=clamp(dot(lightDir,normal),0.0f, 1.0f);
    float VdotN=clamp(dot(viewDir,normal),0.0f, 1.0f);
    float VdotH=clamp(dot(H,viewDir),0.0f, 1.0f);
    float NdotH=clamp(dot(H,normal),0.0f, 1.0f);

    float F=factReflec+((1.0f - factReflec) * pow((1.0f-VdotH),5.0f));

    float NdotH2=NdotH*NdotH;
    float g1 = (2 * NdotH * VdotN) / VdotH;
    float g2 = (2*NdotH * LdotN) / VdotH;
    float G = clamp(min(g1, g2),0.0f,1.0f);

    
    float m2 = objMaterial.roughness * objMaterial.roughness;
    float r1 = 1.0 / (PI * m2 * pow(NdotH, 4));
    float r2 = (NdotH2 - 1.0) / (m2 * NdotH2);
    float D = r1 * exp(r2)* exp(r2);

    float Rs=(F*D*G)/(VdotN*LdotN);
    return Rs;

}

vec3 PointLightCon(PointLight light,vec3 normal, vec3 lightDir, vec3 viewDir, vec3 kd,vec3 ks,int i){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    float LdotN=clamp(dot(lightDir,normal),0.0f, 1.0f);
    float diff=LdotN;
    vec3 diffuse=light.DifusseColor*(diff *kd);
   
    float spec=0.0f;
    if(LdotN>0){
        spec=cookTorranceVal(normal,lightDir,viewDir);
    }
    vec3 specular=light.SpecularColor*(spec * ks);
    
    
    float distance =length(dataIn.positionPL[i]-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);

    
    Contribution=LdotN*(ambient+diffuse+specular) *attenuation;
    return Contribution;
}

vec3 AmbientLightCon(AmbientLight light,vec3 normal, vec3 lightDir, vec3 viewDir, vec3 kd,vec3 ks){
    vec3 Contribution;
    vec3 ambient=light.AmbientColor * objMaterial.AmbientColor;
    float LdotN=clamp(dot(lightDir,normal),0.0f, 1.0f);
    float diff=LdotN;
    vec3 diffuse=light.DifusseColor*(diff *kd);
   
    float spec=0.0f;
    if(LdotN>0){
        spec=cookTorranceVal(normal,lightDir,viewDir);
    }
    vec3 specular=light.SpecularColor*(spec * ks);
    
    Contribution=LdotN *(ambient+diffuse+specular);
    return Contribution;
}

vec3 SpotLightCon(SpotLight light, vec3 normal, vec3 lightDir,vec3 viewDir, vec3 kd,vec3 ks){
    vec3 Contribution;
    
    vec3 ambient=light.AmbientColor* objMaterial.AmbientColor;
    float LdotN=clamp(dot(lightDir,normal),0.0f, 1.0f);
    float diff=LdotN;
    vec3 diffuse=light.DifusseColor*(diff *kd);
   
    float spec=0.0f;
    if(LdotN>0){
        spec=cookTorranceVal(normal,lightDir,viewDir);
    }
    vec3 specular=light.SpecularColor*(spec * ks);
    
    float distance =length(dataIn.positionSP-dataIn.vertexPos);
    float attenuation = Attenuation(light.Constant,light.Linear ,light.Quadratic ,distance);
    float theta = dot(lightDir, normalize(-dataIn.directionSP)); 
    float epsilon = light.CuttOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    Contribution=LdotN*(ambient+diffuse+specular)*attenuation *intensity;
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

    kd*=texture2D(objMaterial.kdTexture, texCoords).rgb;
    
    vec3 ks=objMaterial.SpecularColor;
    ks*=texture2D(objMaterial.ksTexture, texCoords).rgb;



    
    if(ambientLight.Active == 1){
        lightContribution=AmbientLightCon(ambientLight,normal,normalize(-dataIn.directionAL),viewDir,kd,ks);
    }
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        if(pointLight[i].Active==1){
            lightContribution+=PointLightCon(pointLight[i],normal,normalize(dataIn.positionPL[i]-dataIn.vertexPos),viewDir,kd,ks,i);
        }
    }
    if(spotLight.Active==1){
        lightContribution+=SpotLightCon(spotLight,normal,normalize(dataIn.positionSP-dataIn.vertexPos),viewDir,kd,ks);
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


    fragColor= vec4(lightContribution*refl*refr, texture2D(objMaterial.kdTexture, texCoords).a);
}

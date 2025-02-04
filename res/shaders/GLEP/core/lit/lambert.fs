#version 330 core

struct Vertex {
    vec3 position;
    vec3 tangentPosition;
    vec4 lightSpacePosition;
    vec2 uv;
    mat3 tbn;
};

struct GLEPInfo {
    float time;
    float deltaTime;
    vec3 viewPos;
    vec3 tangentViewPos;
};

struct AmbientLight{
    vec4 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec4 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 position;
    vec4 color;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec4 color;
    float intensity;

    float innerCutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    int type;
    bool hasNormalMap;
    bool hasDepthTex;

    vec4 diffuseColor;

    sampler2D diffuseTex;

    sampler2D normalTex;
    sampler2D depthTex;
    float depthScale;
};

struct Framebuffer {
    sampler2D color;
    sampler2D depth;
};

in Vertex v;
in GLEPInfo i;
out vec4 FragColor;

#define MAX_POINT_LIGHTS 50
#define MAX_SPOT_LIGHTS 50

uniform Framebuffer uShadowMap;

uniform Material uMaterial;

uniform AmbientLight uAmbient;
uniform DirectionalLight uDirectionalLight;

uniform int uPointLightsAmt = 0;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

uniform int uSpotLightsAmt = 0;
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS];

vec3 diffuseLighting(vec3 dir, vec3 norm, vec3 lightColor, float intensity, vec3 diffuseMat){
    float diff = max(dot(norm, dir), 0.0);
    return vec3(lightColor * diff * intensity * diffuseMat);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 diffuseMat){
    vec3 lightDir = vec3(0.0f);
    if(uMaterial.hasNormalMap){
        vec3 lightPos = v.tbn * light.position;
        lightDir = normalize(lightPos - v.tangentPosition);
    }else {
        lightDir = normalize(light.position - v.position);
    }

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, diffuseMat);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= attenuation;

    return diffuse;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 diffuseMat){
    vec3 lightDir = vec3(0.0f);
    if(uMaterial.hasNormalMap){
        lightDir = normalize(v.tbn * -light.direction);
    }else {
        lightDir = normalize(-light.direction);
    }

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, diffuseMat);

    return diffuse;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 diffuseMat){
    vec3 lightDir = vec3(0.0f);
    if(uMaterial.hasNormalMap){
        vec3 lightPos = v.tbn * light.position;
        lightDir = normalize(lightPos - v.tangentPosition);
    }else {
        lightDir = normalize(light.position - v.position);
    }

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, diffuseMat);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= intensity;
    diffuse *= attenuation;

    return diffuse;
        
}

float calcDirectionalShadow(vec4 positionLightSpace, vec3 normal){
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(uShadowMap.depth, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 lightDir = vec3(0.0f);
    if(uMaterial.hasNormalMap){
        vec3 lightPos = v.tbn * uDirectionalLight.position;
        lightDir = normalize(lightPos - v.tangentPosition);
    } else {
        lightDir = normalize(uDirectionalLight.position - v.position);
    }

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  

    //PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap.depth, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadowMap.depth, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir){
    const float numLayers = 10;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * uMaterial.depthScale; 
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(uMaterial.depthTex, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(uMaterial.depthTex, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(uMaterial.depthTex, prevTexCoords).r - currentLayerDepth + layerDepth;
    
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main(){
    vec3 result = vec3(0.0f);

    vec4 diffuseMat = vec4(1.0f);

    vec2 texCoords = v.uv;
    if(uMaterial.hasDepthTex){
        vec3 viewDir = normalize(i.tangentViewPos - v.tangentPosition);
        texCoords = parallaxMapping(v.uv, viewDir);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }

    vec3 normal = v.tbn[2];
    if(uMaterial.hasNormalMap){
        normal = texture(uMaterial.normalTex, texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }

    if(uMaterial.type == 1){
        diffuseMat = uMaterial.diffuseColor;
    } else if(uMaterial.type == 2){
        diffuseMat = texture(uMaterial.diffuseTex, texCoords);
    }

    vec3 ambient = uAmbient.color.rgb * uAmbient.intensity * diffuseMat.rgb;

    for(int i = 0; i < uPointLightsAmt; i++){
        result += calcPointLight(uPointLights[i], normal, diffuseMat.rgb);
    }

    for(int i = 0; i < uSpotLightsAmt; i++){
        result += calcSpotLight(uSpotLights[i], normal, diffuseMat.rgb);
    }

    result += calcDirectionalLight(uDirectionalLight, normal, diffuseMat.rgb);

    float shadow = calcDirectionalShadow(v.lightSpacePosition, normal); 
    vec3 lighting = (ambient + (1.0 - shadow) * result); 

    vec4 finalColor = vec4(lighting, diffuseMat.a);
    if(finalColor.a < 0.1f) discard;

    FragColor = finalColor;
}
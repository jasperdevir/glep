#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;
layout (location = 3) out vec3 gSpecular;

struct Vertex {
    vec3 position;
    vec3 tangentPosition;
    vec4 lightSpacePosition;
    vec2 uv;
    vec3 normal;
    mat3 tbn;
};

struct GLEPInfo {
    float time;
    float deltaTime;
    vec3 viewPos;
    vec3 tangentViewPos;
};

struct Material {
    int type;
    bool hasNormalTex;
    bool hasDispTex;

    float shininess;

    vec4 diffuseColor;
    vec4 specularColor;

    sampler2D diffuseTex;
    sampler2D specularTex;

    sampler2D normalTex;
    sampler2D dispTex;
    float dispScale;
};

in Vertex v;
in GLEPInfo i;

uniform Material uMaterial;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir){
    const float numLayers = 10;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * uMaterial.dispScale; 
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(uMaterial.dispTex, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(uMaterial.dispTex, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(uMaterial.dispTex, prevTexCoords).r - currentLayerDepth + layerDepth;
    
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{    
    gPosition = v.position;

    vec2 texCoords = v.uv;
    if(uMaterial.hasDispTex){
        vec3 viewDir = normalize(i.tangentViewPos - v.tangentPosition);
        texCoords = parallaxMapping(v.uv, viewDir);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }

    vec3 normal = normalize(v.normal);
    if(uMaterial.hasNormalTex){
        normal = texture(uMaterial.normalTex, texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }

    gNormal = normal;

    if(uMaterial.type == 1){
        gColor = uMaterial.diffuseColor;
        gSpecular = uMaterial.specularColor.rgb;
    } else if (uMaterial.type == 2){
        gColor = texture(uMaterial.diffuseTex, texCoords);
        gSpecular = uMaterial.specularColor.rgb;
    } else if (uMaterial.type == 3){
        gColor = texture(uMaterial.diffuseTex, texCoords);
        gSpecular = texture(uMaterial.specularTex, texCoords).rgb;
    }
   
}
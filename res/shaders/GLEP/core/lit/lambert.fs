#version 330 core

struct Vertex {
    vec3 position;
    vec4 lightSpacePosition;
    vec3 normal;
    vec2 uv;
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

    vec4 diffuseColor;

    sampler2D diffuseTex;
};

struct Framebuffer {
    sampler2D color;
    sampler2D depth;
};

in Vertex v;
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

vec3 calcPointLight(PointLight light, vec3 diffuseMat){
    vec3 norm = normalize(v.normal);
    vec3 dir = normalize(light.position - v.position); 
    vec3 diffuse = diffuseLighting(dir, norm, light.color.rgb, light.intensity, diffuseMat);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= attenuation;

    return diffuse;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 diffuseMat){
    vec3 norm = normalize(v.normal);
    vec3 dir = normalize(-light.direction);
    vec3 diffuse = diffuseLighting(dir, norm, light.color.rgb, light.intensity, diffuseMat);

    return diffuse;
}

vec3 calcSpotLight(SpotLight light, vec3 diffuseMat){
    vec3 dir = normalize(light.position - v.position); 
    float theta = dot(dir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    vec3 norm = normalize(v.normal);
    vec3 diffuse = diffuseLighting(dir, norm, light.color.rgb, light.intensity, diffuseMat);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= intensity;
    diffuse *= attenuation;

    return diffuse;
        
}

float calcDirectionalShadow(vec4 positionLightSpace){
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(uShadowMap.depth, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(v.normal);
    vec3 lightDir = normalize(uDirectionalLight.position - v.position);
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

void main(){
    vec3 result = vec3(0.0f);

    vec4 diffuseMat = vec4(1.0f);

    if(uMaterial.type == 1){
        diffuseMat = uMaterial.diffuseColor;
    } else if(uMaterial.type == 2){
        diffuseMat = texture(uMaterial.diffuseTex, v.uv);
    }

    vec3 ambient = uAmbient.color.rgb * uAmbient.intensity * diffuseMat.rgb;

    for(int i = 0; i < uPointLightsAmt; i++){
        result += calcPointLight(uPointLights[i], diffuseMat.rgb);
    }

    for(int i = 0; i < uSpotLightsAmt; i++){
        result += calcSpotLight(uSpotLights[i], diffuseMat.rgb);
    }

    result += calcDirectionalLight(uDirectionalLight, diffuseMat.rgb);

    float shadow = calcDirectionalShadow(v.lightSpacePosition); 
    vec3 lighting = (ambient + (1.0 - shadow) * result); 

    vec4 finalColor = vec4(lighting, diffuseMat.a);
    if(finalColor.a < 0.1f) discard;

    FragColor = finalColor;
}
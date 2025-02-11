#version 330 core

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct GLEPInfo {
    float time;
    float deltaTime;
    vec3 viewPos;
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

struct GBuffer{
    sampler2D position;
    sampler2D normal;
    sampler2D diffuse;
    sampler2D specular;
};

struct Framebuffer{
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

uniform GBuffer uGBuffer;

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

vec3 specularLighting(vec3 dir, vec3 norm, vec3 lightColor, float intensity, vec3 specularMat){
    vec3 viewDir = normalize(i.viewPos - v.position);
    vec3 halfwayDir = normalize(dir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), uMaterial.shininess);

    return vec3(lightColor * spec * intensity * specularMat);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 matDiffuse, vec3 matSpecular){
    vec3 lightDir = normalize(light.position - v.position);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, matDiffuse);

    vec3 specular = specularLighting(lightDir, normal, light.color.rgb, light.intensity, matSpecular);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= intensity;
    specular *= intensity;

    diffuse *= attenuation;
    specular *= attenuation; 

    return diffuse + specular;
        
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 matDiffuse, vec3 matSpecular){
    vec3 lightDir = normalize(-light.direction);

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, matDiffuse);

    vec3 specular = specularLighting(lightDir, normal, light.color.rgb, light.intensity, matSpecular);

    return diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 matDiffuse, vec3 matSpecular){
    vec3 lightDir = normalize(light.position - v.position);

    vec3 diffuse = diffuseLighting(lightDir, normal, light.color.rgb, light.intensity, matDiffuse);

    vec3 specular = specularLighting(lightDir, normal, light.color.rgb, light.intensity, matSpecular);

    float dist = length(light.position - v.position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= attenuation;
    specular *= attenuation;     

    return diffuse + specular;
}

float calcDirectionalShadow(vec4 positionLightSpace, vec3 normal){
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(uShadowMap.depth, projCoords.xy).r; 
    float currentDepth = projCoords.z;

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
   
    vec4 matDiffuse = texture(uGBuffer.diffuse, v.uv);
    vec3 matSpecular = texture(uGBuffer.specular, v.uv).rgb;

    vec3 gPosition = texture(uGBuffer.position, v.uv).rgb;
    vec3 normal = texture(uGBuffer.normal, v.uv).rgb;
    
    vec3 ambient = uAmbient.color.rgb * uAmbient.intensity * matDiffuse.rgb;

    for(int i = 0; i < uPointLightsAmt; i++){
        result += calcPointLight(uPointLights[i], normal, matDiffuse.rgb, matSpecular);
    }

    for(int i = 0; i < uSpotLightsAmt; i++){
        result += calcSpotLight(uSpotLights[i], normal, matDiffuse.rgb, matSpecular);
    }

    //result += calcDirectionalLight(uDirectionalLight, normal, matDiffuse.rgb, matSpecular);

    //float shadow = calcDirectionalShadow(v.lightSpacePosition, normal); 
    vec3 lighting = (ambient + result); 
    
    vec4 finalColor = vec4(lighting, matDiffuse.a);
    if(finalColor.a < 0.1f) discard; 

    FragColor = finalColor;

}
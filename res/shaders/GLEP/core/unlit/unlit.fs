#version 330 core

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

struct Material{
    int type;
    
    vec4 diffuseColor;

    sampler2D diffuseTex;
};

in Vertex v;
in GLEPInfo i;
out vec4 FragColor;

uniform Material uMaterial;

void main(){
    vec4 finalColor = vec4(1.0f);

    if(uMaterial.type == 1){
        finalColor = uMaterial.diffuseColor;
    } else if (uMaterial.type == 2){
        finalColor = texture(uMaterial.diffuseTex, v.uv);
    }

    if(finalColor.a < 0.1f) discard;

    FragColor = finalColor;
}
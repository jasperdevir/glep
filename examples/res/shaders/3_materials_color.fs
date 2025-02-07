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
    vec4 color;
};

in Vertex v;
in GLEPInfo i;
out vec4 FragColor;

uniform Material uMaterial;

void main(){
    vec3 finalColor = uMaterial.color.rgb * v.uv.x;
    float r = (sin(i.time * 0.5f) + 1.0f) * 0.5f;
    float g = (sin(i.time) + 1.0f) * 0.5f;
    float b = (sin(i.time * 1.5f) + 1.0f) * 0.5f;
    finalColor *= vec3(r,g,b);
    FragColor = vec4(finalColor, 1.0f);
}
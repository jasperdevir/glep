#version 330 core

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 uv;
};

struct Material{
    samplerCube cubeMap;
};

in Vertex v;
out vec4 FragColor;

uniform Material uMaterial;

void main(){
    vec3 color = texture(uMaterial.cubeMap, v.uv).rgb;
    FragColor = vec4(color, 1.0f);
}
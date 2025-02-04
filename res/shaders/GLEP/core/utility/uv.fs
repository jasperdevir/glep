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

in Vertex v;
out vec4 FragColor;

void main(){
    FragColor = vec4(v.uv, 1.0f, 1.0f);
}
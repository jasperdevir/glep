#version 330 core

struct Vertex {
    vec3 position;
    vec4 lightSpacePosition;
    vec3 normal;
    vec2 uv;
};

in Vertex v;
out vec4 FragColor;

void main(){
    FragColor = vec4(v.uv, 1.0f, 1.0f);
}
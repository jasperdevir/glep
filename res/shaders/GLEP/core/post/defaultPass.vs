#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

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

out Vertex v;
out GLEPInfo i;

uniform float time;
uniform float deltaTime;
uniform vec3 viewPos;

void main(){
    v.position = aPos;
    v.normal = aNormal;  
    v.uv = aTexCoords;

    i.time = time;
    i.deltaTime = deltaTime;
    i.viewPos = viewPos;

    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct Vertex {
    vec3 position;
    vec4 lightSpacePosition;
    vec3 normal;
    vec2 uv;
};

struct GLEPInfo {
    float time;
    float deltaTime;
    vec3 viewPos;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform float time;
uniform float deltaTime;
uniform vec3 viewPos;

out Vertex v;
out GLEPInfo i;

void main(){
    v.position = vec3(model * vec4(aPos, 1.0));
    v.normal = mat3(transpose(inverse(model))) * aNormal;  
    v.uv = aTexCoords;
    v.lightSpacePosition = lightSpaceMatrix * vec4(v.position, 1.0);

    i.time = time;
    i.deltaTime = deltaTime;
    i.viewPos = viewPos;

    gl_Position = projection * view * vec4(v.position, 1.0);
}
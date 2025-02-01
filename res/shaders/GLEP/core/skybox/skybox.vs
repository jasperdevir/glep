#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 uv;
};

uniform mat4 view;
uniform mat4 projection;

out Vertex v;

void main(){
    v.position = aPos;
    v.normal = aNormal;  
    v.uv = aPos;

    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
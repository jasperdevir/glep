#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

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
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v.tbn = transpose(mat3(T,B,N));
    v.tangentPosition = v.tbn * v.position;

    i.time = time;
    i.deltaTime = deltaTime;
    i.viewPos = viewPos;
    i.tangentViewPos = v.tbn * i.viewPos;

    gl_Position = projection * view * vec4(v.position, 1.0);
}
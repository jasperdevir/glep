#version 330 core

struct Vertex {
    vec3 position;
    vec4 lightSpacePosition;
    vec3 normal;
    vec2 uv;
};

in Vertex v;
out vec4 FragColor;

struct Material{
    float nearPlane;
    float farPlane;
    float resultScale;
};

uniform Material uMaterial;

void main(){
    float depth = gl_FragCoord.z;

    float z = depth * 2.0 - 1.0;
    float ndc = (2.0 * uMaterial.nearPlane * uMaterial.farPlane) / (uMaterial.farPlane + uMaterial.nearPlane - z * (uMaterial.farPlane - uMaterial.nearPlane));	

    float result = ndc / uMaterial.farPlane * uMaterial.resultScale;
    
    FragColor = vec4(vec3(result), 1.0);
}
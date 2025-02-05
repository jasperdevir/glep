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

struct Material {
    samplerCube cubeMap;
    vec4 tint;
};

in Vertex v;
in GLEPInfo i;
out vec4 FragColor;

uniform Material uMaterial;

void main()
{             
    vec3 I = normalize(v.position - i.viewPos);
    vec3 R = reflect(I, normalize(v.normal));

    vec3 finalColor = texture(uMaterial.cubeMap, R).rgb;
    finalColor *= uMaterial.tint.rgb;

    FragColor = vec4(finalColor, 1.0);
}